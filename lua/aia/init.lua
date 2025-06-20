local M = {}

local ui = require("aia.ui");
local websocket = require("aia.websocket_client");

M.setup_aia = function()
    vim.api.nvim_create_autocmd("User", {
        pattern = "OnPromptSubmit",
        callback = function(event)
            local prompt = event.data.input
            websocket.send_prompt(prompt)
        end,
    })
    websocket.start()
end

vim.api.nvim_create_user_command("SetupAia", M.setup_aia, { desc = "test" })

return M
