local M = {}

local ui = require("aia.ui");
local websocket = require("aia.websocket_client");
local ws = nil

M.setup_aia = function()
    M.prompt_listener()
    websocket.start()
    ws = websocket.create_connection()
end

M.prompt_listener = function ()
    vim.api.nvim_create_autocmd("User", {
        pattern = "OnPromptSubmit",
        callback = function(event)
            local prompt = event.data.input
            websocket.send_prompt(ws, prompt)
        end,
    })
end

M.prompt_listener()

vim.api.nvim_create_user_command("SetupAia", M.setup_aia, { desc = "test" })

return M
