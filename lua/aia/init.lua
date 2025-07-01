local M = {}

local ui = require("aia.ui");
local tcp = require("aia.tcp_client");
local _ = require("aia.project_manager");
local context = require("aia.context");


M.setup_aia = function()
    vim.api.nvim_create_autocmd("User", {
        pattern = "OnPromptSubmit",
        callback = function(event)
            local prompt = event.data.input
            local harpoon = context.get_harpoon_context()
            local git_diff = context.get_git_diff()
            local request = { prompt = prompt, harpoon_files = harpoon, git_diff = git_diff }
            tcp.write_prompt(request)
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
