local websocket = require 'websocket.client'
local copas = require 'copas'

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
    local ok, err = ws:send('Hello from Lua!')
    if ok then
        print('Sent: Hello from Lua!')
    else
        print('Send error: ' .. (err or 'Unknown error'))
    end

    -- Receive messages in a loop
    while true do
        local message, err = ws:receive()
        if message then
            print('Received: ' .. message)
        elseif err == 'closed' then
            print('Connection closed: ' .. (err or 'Unknown error'))
            break
        elseif err then
            print('Receive error: ' .. (err or 'Unknown error'))
        end
        copas.sleep(0) -- Yield to allow other tasks
    end
end

-- Add the WebSocket task to copas
copas.addthread(websocket_task)

-- Run the event loop
copas.loop()
