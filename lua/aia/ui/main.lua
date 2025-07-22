local M = {}

local floating_win_module = require("aia.ui.floating_assistant")
local project_win_module = require("aia.ui.project_ui")

local cur_win = nil
local floating_win = nil
local project_win = nil

M.create_floating_win = function()
    if not floating_win then
        floating_win = floating_win_module.new()
    end
    cur_win = floating_win
    floating_win:create_floating_win()
end

M.create_project_win = function()
    if not project_win then
        project_win = project_win_module.new()
    end
    cur_win = project_win
    project_win:create_project_win()
end



M.response = function(response)
    if cur_win and cur_win.response then
        cur_win:response(response)
    end
end

return M
