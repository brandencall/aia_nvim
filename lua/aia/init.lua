local M = {}

local ui = require("aia.ui");
local floating_ui = require("aia.ui.main");
local tcp = require("aia.tcp_client");
local _ = require("aia.project_manager");
local context = require("aia.context");


M.setup_aia = function()
    vim.api.nvim_create_autocmd("User", {
        pattern = "OnPromptSubmit",
        callback = function(event)
            local prompt = event.data.input
            tcp.write_prompt(context.get_project_context(prompt))
        end,
    })
    vim.api.nvim_create_autocmd("User", {
        pattern = "ServerResponse",
        callback = function(event)
            local response = event.data.response
            --ui.set_content_text(response)
            floating_ui.ai_response(response)
        end,
    })
    tcp.connect_tcp()
    vim.api.nvim_set_hl(0, "AIAUserMessageBg", { bg = "#4A4A4A", fg = "#FFFFFF" }) -- Dark gray background for user
    vim.api.nvim_set_hl(0, "AIAAiMessageBg", { bg = "#3B5998", fg = "#FFFFFF" }) -- Blue background for AI
    vim.api.nvim_set_hl(0, "AIAContentWinBorder", { fg = "#808080" })          -- For content window border
end

vim.api.nvim_create_user_command("SetupAia", M.setup_aia, { desc = "test" })

return M
