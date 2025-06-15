local M = {}

M.autoComplete = function()
    local cursorPos = vim.api.nvim_win_get_cursor(0)
    local row, col = cursorPos[1] - 1, cursorPos[2]
    local lines = vim.api.nvim_buf_get_lines(0, 0, row - 1, false)
    local current_line = lines[#lines]:sub(1, col)
end


return M
