local M = {}

M.is_connected = false

function M.set_connected(status)
    M.is_connected = status
end

function M.get_status()
    return M.is_connected
end

function M.get_status_icon()
    return M.is_connected and " Connected" or " Disconnected"
end

function M.update_status_winbar()
    vim.schedule(function()
        vim.cmd("redrawstatus")
    end)
end

return M
