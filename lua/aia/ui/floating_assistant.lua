local M = {}

local message_bubble = require("aia.ui.message_bubble")
local connection_state = require("aia.state")
local tcp = require("aia.tcp_client")

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
    local width = math.floor(vim.o.columns * 0.8)
    local height = math.floor(vim.o.lines * 0.9)

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

local function create_content_win(self)
    local content_height = math.floor(vim.api.nvim_win_get_height(self.state.parent_win) * .80)
    local content_opts = {
        relative = "win",
        win = self.state.parent_win,
        width = vim.api.nvim_win_get_width(self.state.parent_win) - 4,
        height = content_height,
        row = 1,
        col = 1,
        style = "minimal",
        border = "rounded",
        title = " AI Assistant ",
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
    vim.api.nvim_buf_set_option(self.state.content_buf, 'filetype', 'markdown')
end

local function create_prompt_win(self)
    local prompt_height = 3
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
        title = " Prompt ",
        title_pos = "left",
    })
    vim.api.nvim_buf_set_lines(self.state.prompt_buf, 0, -1, false, { "" })
    vim.api.nvim_set_option_value("wrap", true, { win = self.state.prompt_win })
    vim.api.nvim_set_option_value("linebreak", true, { win = self.state.prompt_win })

    vim.api.nvim_set_current_win(self.state.prompt_win)
    vim.cmd("startinsert")
end

local function add_user_prompt(self, prompt)
    if not self.state.content_win or not self.state.content_buf then
        vim.notify("Content window not created yet.", vim.log.ERROR)
        return
    end
    local content_win_width = vim.api.nvim_win_get_width(self.state.content_win)
    message_bubble.append_user_message(self.state.content_buf, prompt, content_win_width)
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

M.create_floating_win = function(self)
    if not connection_state.is_connected then
        tcp.connect_tcp()
    end
    create_parent_win(self)
    create_content_win(self)
    create_prompt_win(self)

    M.setup_auto_close(self)
    M.on_submit(self)
end

function M:close_windows()
    local windows_to_close = {
        self.state.parent_win,
        self.state.content_win,
        self.state.prompt_win,
    }
    for _, win_id in ipairs(windows_to_close) do
        if vim.api.nvim_win_is_valid(win_id) then
            vim.api.nvim_win_close(win_id, true)
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

M.on_submit = function(self)
    vim.keymap.set("i", "<CR>", function()
        local prompt = vim.api.nvim_buf_get_lines(self.state.prompt_buf, 0, -1, false)
        local prompt_request = table.concat(prompt, "\n")

        vim.schedule(function()
            add_user_prompt(self, prompt)
        end)
        vim.schedule(function()
            vim.api.nvim_buf_set_lines(self.state.prompt_buf, 0, -1, false, { "" })
        end)

        vim.api.nvim_exec_autocmds("User", {
            pattern = "OnPromptSubmit",
            data = { input = prompt_request },
        })
    end, { buffer = self.state.prompt_buf, noremap = true, silent = true, expr = true })
end


return M
