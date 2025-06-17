local M = {}


M.autoComplete = function()
    local cursorPos = vim.api.nvim_win_get_cursor(0)
    local row = cursorPos[1]
    local prefix = vim.api.nvim_buf_get_lines(0, 0, row, false)
    local prompt = table.concat(prefix, "\n")
    local suffix = vim.api.nvim_buf_get_lines(0, row, -1, false)

    local request = vim.fn.json_encode({
        prompt = prefix,
        suffix = suffix,
        n_predict = 100,
    })
    local curl_cmd = string.format(
        "curl -X POST http://brabs@server.brabs:8080/completion -H 'Content-Type: application/json' -d '%s'", request)
    local response = vim.fn.system(curl_cmd)
    print(response)
end

M.testConnection = function()
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


vim.api.nvim_create_user_command("AiaTest", M.autoComplete, { desc = "test" })
vim.api.nvim_create_user_command("ConTest", M.testConnection, { desc = "test" })

return M
