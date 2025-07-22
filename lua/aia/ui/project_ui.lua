local M = {}

local tcp = require("aia.tcp_client");
local message_bubble = require("aia.ui.message_bubble")
local connection_state = require("aia.state")

function M.new()
    local self = setmetatable({}, { __index = M })
    self.state = {
        parent_win = nil,
        parent_buf = nil,
        content_win = nil,
        content_buf = nil,
        prompt_win = nil,
        prompt_buf = nil,
    }
    return self
end

local function create_parent_win(self)
    local width = math.floor(vim.o.columns * 0.5)
    local height = math.floor(vim.o.lines * 0.7)

    local row = math.floor((vim.o.lines - height) / 3)
    local col = math.floor((vim.o.columns - width) / 2)

    self.state.parent_buf = vim.api.nvim_create_buf(false, true)
    self.state.parent_win = vim.api.nvim_open_win(self.state.parent_buf, false, {
        relative = "editor",
        width = width,
        height = height,
        row = row,
        col = col,
        style = "minimal",
        border = "none",
    })
end

---@param project_id string The project_id for the current project
local function create_content_win(self, project_id)
    local content_height = math.floor(vim.api.nvim_win_get_height(self.state.parent_win) * .6)
    local content_opts = {
        relative = "win",
        win = self.state.parent_win,
        width = vim.api.nvim_win_get_width(self.state.parent_win) - 4,
        height = content_height,
        row = 1,
        col = 1,
        style = "minimal",
        border = "rounded",
        title = " " .. project_id .. " ",
        title_pos = "center",
    }

    if self.state.content_buf and vim.api.nvim_buf_is_valid(self.state.content_buf) then
        self.state.content_win = vim.api.nvim_open_win(self.state.content_buf, true, content_opts)
        return
    end
    self.state.content_buf = vim.api.nvim_create_buf(false, true)
    self.state.content_win = vim.api.nvim_open_win(self.state.content_buf, true, content_opts)
    vim.api.nvim_set_option_value("wrap", true, { win = self.state.content_win })
    vim.api.nvim_set_option_value("linebreak", true, { win = self.state.content_win })
    vim.api.nvim_set_option_value("breakindent", true, { win = self.state.content_win })
    vim.api.nvim_buf_set_option(self.state.content_buf, "bufhidden", "hide")
end

local function create_prompt_win(self)
    local prompt_height = math.floor(vim.api.nvim_win_get_height(self.state.parent_win) * .25)
    self.state.prompt_buf = vim.api.nvim_create_buf(false, true)
    self.state.prompt_win = vim.api.nvim_open_win(self.state.prompt_buf, true, {
        relative = "win",
        win = self.state.parent_win,
        width = vim.api.nvim_win_get_width(self.state.parent_win) - 4,
        height = prompt_height,
        row = vim.api.nvim_win_get_height(self.state.content_win) + 3,
        col = 1,
        style = "minimal",
        border = "rounded",
        title = " Project Context ",
        title_pos = "left",
    })
    vim.api.nvim_buf_set_lines(self.state.prompt_buf, 0, -1, false, { "" })
    vim.api.nvim_set_option_value("wrap", true, { win = self.state.prompt_win })
    vim.api.nvim_set_option_value("linebreak", true, { win = self.state.prompt_win })

    vim.api.nvim_set_current_win(self.state.prompt_win)
    vim.cmd("startinsert")
end


--- @treturn string returns the project root that will be used as the project id
local function get_project_id()
    local project_root = vim.fs.root(0, ".git")
    if project_root == nil then
        print("The project needs to be a .git directory")
        return ""
    end
    return project_root
end

local function open_windows(self, project_id)
    create_parent_win(self)
    create_content_win(self, project_id)
    create_prompt_win(self)
end

M.response = function(self, response)
    if not self.state.content_win or not self.state.content_buf then
        vim.notify("Content window not created yet.", vim.log.ERROR)
        return
    end
    local lines = {}
    for line in response:gmatch("[^\n]+") do
        table.insert(lines, line)
    end
    message_bubble.append_ai_message(self.state.content_buf, lines)
end

M.create_project_win = function(self)
    -- try to connect to proxy (floating_assistant)
    if not connection_state.is_connected then
        tcp.connect_tcp()
    end
    local project_id = get_project_id()
    if project_id == "" then
        return
    end
    open_windows(self, project_id)
    tcp.get_project(project_id)

    M.setup_auto_close(self)
    M.on_submit(self, project_id)
end

function M:close_windows()
    local windows_to_close = {
        self.state.parent_win,
        self.state.content_win,
        self.state.prompt_win,
    }
    local buffers_to_delete = {
        self.state.parent_buf,
        self.state.content_buf,
        self.state.prompt_buf,
    }
    for _, win_id in ipairs(windows_to_close) do
        if vim.api.nvim_win_is_valid(win_id) then
            vim.api.nvim_win_close(win_id, true)
        end
    end
    for _, buf_id in ipairs(buffers_to_delete) do
        if vim.api.nvim_buf_is_valid(buf_id) then
            vim.api.nvim_buf_delete(buf_id, { force = true })
        end
    end
end

M.setup_auto_close = function(self)
    vim.api.nvim_create_autocmd("WinClosed", {
        -- Need to detect it on the user_win and the ai_win
        pattern = { tostring(self.state.content_win), tostring(self.state.prompt_win), tostring(self.state.parent_win) },
        callback = function()
            self:close_windows()
        end,
        once = true, -- Run only once to avoid repeated triggers
    })
end

M.on_submit = function(self, project_id)
    vim.keymap.set("i", "<CR>", function()
        local prompt = vim.api.nvim_buf_get_lines(self.state.prompt_buf, 0, -1, false)
        local prompt_request = table.concat(prompt, "\n")

        if string.len(prompt_request) == 0 then
            return
        end

        vim.schedule(function()
            vim.api.nvim_buf_set_lines(self.state.prompt_buf, 0, -1, false, { "" })
            vim.api.nvim_buf_set_lines(self.state.content_buf, 0, -1, false, { "" })
        end)

        vim.api.nvim_exec_autocmds("User", {
            pattern = "OnProjectContextSubmit",
            data = { input = prompt_request, project_id = project_id },
        })
    end, { buffer = self.state.prompt_buf, noremap = true, silent = true, expr = true })
end

return M
