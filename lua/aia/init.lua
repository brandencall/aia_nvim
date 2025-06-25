local M = {}

local ui = require("aia.ui");
local tcp = require("aia.tcp_client");
local proj_manager = require("aia.project_manager");


M.setup_aia = function()
    vim.api.nvim_create_autocmd("User", {
        pattern = "OnPromptSubmit",
        callback = function(event)
            local prompt = event.data.input
            tcp.write_prompt(prompt)
        end,
    })
    vim.api.nvim_create_autocmd("User", {
        pattern = "ServerResponse",
        callback = function(event)
            local response = event.data.response
            ui.set_content_text(response)
        end,
    })
    tcp.connect_tcp()
end

vim.api.nvim_create_user_command("SetupAia", M.setup_aia, { desc = "test" })

return M
