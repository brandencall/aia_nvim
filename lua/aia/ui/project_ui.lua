local M = {}

local tcp = require("aia.tcp_client");
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

--- @treturn string returns the project root that will be used as the project id
local function get_project_id()
    local project_root = vim.fs.root(0, ".git")
    if project_root == nil then
        print("The project needs to be a .git directory")
        return ""
    end
    return project_root
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
    -- create the project windows and buffers
    create_parent_win(self)
    -- get the project_root
    -- send root to proxy and get the current prompt (if one exists)
    tcp.get_project(project_id)
    -- upsert the project with the new prompt if one is entered
end

return M
