local M = {}

local websocket = require 'websocket.client'
local copas = require 'copas'

local function log(msg)
    vim.schedule(function()
        print(msg)
    end)
end

M.create_connection = function()
    local ws_connection = websocket.copas()

    local function connect()
        local ok, err = ws_connection:connect('ws://server.brabs:9002')
        if not ok then
            log('Failed to connect: ' .. (err or 'Unknown error'))
            return
        end
        log('Connected to ws://server.brabs:9002')
        copas.sleep(0) -- Yield to allow other tasks
    end

    copas.addthread(connect)
    copas.loop()
    return ws_connection
end

M.send_prompt = function(ws, prompt)
    if not ws or ws.state ~= 'OPEN' then
        print('connection not established yet')
    end

    local function send_to_server()
        local ok, err = ws:send(prompt)
        if ok then
            log('Sent: ' .. prompt)
            return true
        else
            log('Send error: ' .. (err or 'Unknown error'))
            return false
        end
        copas.sleep(0)
    end

    copas.addthread(send_to_server)
    copas.loop()
end

M.start = function()
    M.create_connection()
end

M.testConnection = function(input)
    -- Create an asynchronous WebSocket client
    local ws = websocket.copas()

    -- Define the WebSocket task
    local function websocket_task()
        -- Connect to WebSocket server
        local ok, err = ws:connect('ws://server.brabs:9002')
        if not ok then
            print('Failed to connect: ' .. (err or 'Unknown error'))
            return
        end
        print('Connected to ws://server.brabs:9002')

        -- Send a test message
        local ok, err = ws:send(input)
        if ok then
            print('Sent: Hello from Lua!')
        else
            print('Send error: ' .. (err or 'Unknown error'))
        end

        local message, err = ws:receive()
        if message then
            print('Received: ' .. message)
        elseif err == 'closed' then
            print('Connection closed: ' .. (err or 'Unknown error'))
        elseif err then
            print('Receive error: ' .. (err or 'Unknown error'))
        end
        copas.sleep(0) -- Yield to allow other tasks
    end

    -- Add the WebSocket task to copas
    copas.addthread(websocket_task)

    -- Run the event loop
    copas.loop()
end

return M
