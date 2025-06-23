local ui = require("aia.ui");

require("aia").setup_aia()
vim.api.nvim_create_user_command("SideWin", ui.sideWin, { desc = "test" })
