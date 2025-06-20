local M = {}

local websocket = require 'websocket.client'
local copas = require 'copas'

local ws_connection = nil
local timer = nil

local function log(msg)
    vim.schedule(function()
        print(msg)
    end)
end

M.create_connection = function()
    ws_connection = websocket.copas()

    local function connect()
        local ok, err = ws_connection:connect('ws://server.brabs:9002')
        if not ok then
            log('Failed to connect: ' .. (err or 'Unknown error'))
            return
        end
        log('Connected to ws://server.brabs:9002')
        copas.sleep(0) -- Yield to allow other tasks
        return ws_connection
    end

    copas.addthread(connect)
end

M.send_prompt = function(prompt)
    if not ws_connection or ws_connection.state ~= 'OPEN' then
        print('connection not established yet')
    end

    P(ws_connection)
    local function send_to_server()
        ---@diagnostic disable-next-line: undefined-field
        local ok, err = ws_connection:send(prompt)
        if ok then
            log('Sent: ' .. prompt)
        else
            log('Send error: ' .. (err or 'Unknown error'))
        end
        copas.sleep(0)
    end

    copas.addthread(send_to_server)
end

M.close = function()
    if ws_connection and ws_connection.state == 'OPEN' then
        ---@diagnostic disable-next-line: undefined-field
        ws_connection:close()
        ws_connection = nil
    end
    if timer then
        timer:stop()
        timer:close()
        timer = nil
    end
end

M.start = function()
    M.create_connection()
    vim.api.nvim_create_autocmd('VimLeave', {
        callback = function()
            M.close()
        end,
    })
    if not timer then
        timer = vim.loop.new_timer()
        timer:start(0, 10, vim.schedule_wrap(function()
            copas.step(0.01) -- Process Copas tasks for 10ms
        end))
    end
end

return M
