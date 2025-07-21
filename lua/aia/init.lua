local M = {}

local floating_ui = require("aia.ui.main");
local tcp = require("aia.tcp_client");
local _ = require("aia.project_manager");
local context = require("aia.context");


M.setup_aia = function()
    vim.api.nvim_create_autocmd("User", {
        pattern = "OnPromptSubmit",
        callback = function(event)
            local prompt = event.data.input
            vim.schedule(function()
                local prompt_context = context.get_project_context(prompt)
                tcp.write_prompt(prompt_context)
            end)
        end,
    })
    vim.api.nvim_create_autocmd("User", {
        pattern = "ServerResponse",
        callback = function(event)
            local response = event.data.response
            floating_ui.response(response)
        end,
    })
    tcp.connect_tcp()
end

vim.api.nvim_create_user_command("SetupAia", M.setup_aia, { desc = "test" })

return M
