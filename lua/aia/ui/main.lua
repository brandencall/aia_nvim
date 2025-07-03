local M = {}

local MessageBubble = require("aia.ui.message_bubble")

local state = {
    parent_win = nil,
    parent_buf = nil,
    content_win = nil,
    content_buf = nil,
    prompt_win = nil,
    prompt_buf = nil,
    user_win = nil,
    user_buf = nil,
    ai_win = nil,
    ai_buf = nil,
}

local function create_parent_win()
    local width = math.floor(vim.o.columns * 0.8)
    local height = math.floor(vim.o.lines * 0.9)

    local row = math.floor((vim.o.lines - height) / 3)
    local col = math.floor((vim.o.columns - width) / 2)

    -- Create parent container with border
    state.parent_buf = vim.api.nvim_create_buf(false, true)
    state.parent_win = vim.api.nvim_open_win(state.parent_buf, false, {
        relative = "editor",
        width = width,
        height = height,
        row = row,
        col = col,
        style = "minimal",
        border = "none",
    })
end

local function create_content_win()
    local content_height = math.floor(vim.api.nvim_win_get_height(state.parent_win) * .85)

    state.content_buf = vim.api.nvim_create_buf(false, true)
    state.content_win = vim.api.nvim_open_win(state.content_buf, true, {
        relative = "win",
        win = state.parent_win,
        width = vim.api.nvim_win_get_width(state.parent_win) - 4,
        height = content_height,
        row = 1,
        col = 1,
        style = "minimal",
        border = "rounded",
        title = " AI Assistant ",
        title_pos = "center",
    })
end

local function create_prompt_win()
    local prompt_height = 1
    state.prompt_buf = vim.api.nvim_create_buf(false, true)
    state.prompt_win = vim.api.nvim_open_win(state.prompt_buf, true, {
        relative = "win",
        win = state.parent_win,
        width = vim.api.nvim_win_get_width(state.parent_win) - 4,
        height = prompt_height,
        row = vim.api.nvim_win_get_height(state.content_win) + 3,
        col = 1,
        style = "minimal",
        border = "rounded",
        title = " Prompt ",
        title_pos = "left",
    })
    vim.api.nvim_buf_set_lines(state.prompt_buf, 0, -1, false, { "" })

    -- Start insert mode in prompt
    vim.api.nvim_set_current_win(state.prompt_win)
    vim.cmd("startinsert")
end

local function create_user_win()
    local test_prompt = {
        "How do I create a floating window?",
        "How do I create a floating window?",
        "How do I create a floating window?",
        "How do I create a floating window?",
    }
    local user_bubble = MessageBubble:new({
        lines = test_prompt,
        type = "user",
        relative = "win",
        win = state.content_win,
        width = math.floor((vim.api.nvim_win_get_width(state.content_win)) * 0.4),
        row = 1,
        col = math.floor((vim.api.nvim_win_get_width(state.content_win)) * 0.6) - 2,
    })
    state.user_buf = user_bubble.buf
    state.user_win = user_bubble.win
end

local function create_ai_win()
    local ai_response = {
        "Here's how you create a floating window in Neovim.",
        "You use `nvim_open_win()` with style='minimal'",
        "and position it with relative='editor' or 'win'.",
    }
    local ai_bubble = MessageBubble:new({
        lines = ai_response,
        type = "ai",
        relative = "win",
        win = state.content_win,
        width = vim.api.nvim_win_get_width(state.content_win) - 6,
        row = vim.api.nvim_win_get_height(state.user_win) + 3,
        col = 1,
    })
    state.ai_buf = ai_bubble.buf
    state.ai_win = ai_bubble.win
end

M.create_floating_win = function()
    create_parent_win()
    create_content_win()
    create_prompt_win()
    create_user_win()
    create_ai_win()

    M.setup_auto_close()
end

M.close_windows = function()
    if vim.api.nvim_win_is_valid(state.parent_win) then
        vim.api.nvim_win_close(state.parent_win, true)
    end
    if vim.api.nvim_win_is_valid(state.content_win) then
        vim.api.nvim_win_close(state.content_win, true)
    end
    if vim.api.nvim_win_is_valid(state.prompt_win) then
        vim.api.nvim_win_close(state.prompt_win, true)
    end
    if vim.api.nvim_win_is_valid(state.user_win) then
        vim.api.nvim_win_close(state.user_win, true)
    end
    if vim.api.nvim_win_is_valid(state.ai_win) then
        vim.api.nvim_win_close(state.ai_win, true)
    end
    if vim.api.nvim_buf_is_valid(state.parent_buf) then
        vim.api.nvim_buf_delete(state.parent_buf, { force = true })
    end
    if vim.api.nvim_buf_is_valid(state.content_buf) then
        vim.api.nvim_buf_delete(state.content_buf, { force = true })
    end
    if vim.api.nvim_buf_is_valid(state.prompt_buf) then
        vim.api.nvim_buf_delete(state.prompt_buf, { force = true })
    end
    if vim.api.nvim_buf_is_valid(state.user_buf) then
        vim.api.nvim_buf_delete(state.user_buf, { force = true })
    end
    if vim.api.nvim_buf_is_valid(state.ai_buf) then
        vim.api.nvim_buf_delete(state.ai_buf, { force = true })
    end
end

M.setup_auto_close = function()
    vim.api.nvim_create_autocmd("WinClosed", {
        -- Need to detect it on the user_win and the ai_win
        pattern = { tostring(state.content_win), tostring(state.prompt_win), tostring(state.parent_win) },
        callback = M.close_windows,
        once = true, -- Run only once to avoid repeated triggers
    })
end

vim.api.nvim_create_user_command('FloatingWin', M.create_floating_win, {})

return M
