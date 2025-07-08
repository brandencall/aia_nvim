local ui = require("aia.ui.main");

require("aia").setup_aia()
vim.api.nvim_create_user_command('AiFloatingWin', ui.create_floating_win, {})
