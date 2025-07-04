local M = {}

local message_bubble = require("aia.ui.message_bubble")

local message_top_padding = 3
local message_padding_right = 3

local state = {
    parent_win = nil,
    parent_buf = nil,
    content_win = nil,
    content_buf = nil,
    prompt_win = nil,
    prompt_buf = nil,
    -- user_win = nil,
    -- user_buf = nil,
    -- ai_win = nil,
    -- ai_buf = nil,
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
    vim.api.nvim_set_option_value("wrap", true, { win = state.content_win })
    vim.api.nvim_set_option_value("linebreak", true, { win = state.content_win })
    vim.api.nvim_set_option_value("breakindent", true, { win = state.content_win })
    vim.api.nvim_buf_set_option(state.content_buf, 'filetype', 'markdown')
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

local function create_user_win(prompt)
    if not state.content_win or not state.content_buf then
        vim.notify("Content window not created yet.", vim.log.ERROR)
        return
    end
    -- Get current width of the content window to pass to MessageBubble
    local content_win_width = vim.api.nvim_win_get_width(state.content_win)
    message_bubble.append_message(state.content_buf, prompt, "user", content_win_width)
end

M.ai_response = function(response)
    if not state.content_win or not state.content_buf then
        vim.notify("Content window not created yet.", vim.log.ERROR)
        return
    end
    local lines = {}
    for line in response:gmatch("[^\n]+") do
        table.insert(lines, line)
    end
    -- Get current width of the content window to pass to MessageBubble
    local content_win_width = vim.api.nvim_win_get_width(state.content_win)
    message_bubble.append_message(state.content_buf, lines, "ai", content_win_width)
end

M.create_floating_win = function()
    create_parent_win()
    create_content_win()
    create_prompt_win()

    M.setup_auto_close()
    M.on_submit()
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
    if vim.api.nvim_buf_is_valid(state.parent_buf) then
        vim.api.nvim_buf_delete(state.parent_buf, { force = true })
    end
    if vim.api.nvim_buf_is_valid(state.content_buf) then
        vim.api.nvim_buf_delete(state.content_buf, { force = true })
    end
    if vim.api.nvim_buf_is_valid(state.prompt_buf) then
        vim.api.nvim_buf_delete(state.prompt_buf, { force = true })
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

M.on_submit = function()
    vim.keymap.set("i", "<CR>", function()
        local prompt = vim.api.nvim_buf_get_lines(state.prompt_buf, 0, -1, false)
        local prompt_request = table.concat(prompt, "\n")

        vim.schedule(function()
            create_user_win(prompt)
        end)
        vim.schedule(function()
            vim.api.nvim_buf_set_lines(state.prompt_buf, 0, -1, false, { "" })
        end)

        vim.api.nvim_exec_autocmds("User", {
            pattern = "OnPromptSubmit",
            data = { input = prompt_request },
        })
    end, { buffer = state.prompt_buf, noremap = true, silent = true, expr = true })
end

vim.api.nvim_create_user_command('FloatingWin', M.create_floating_win, {})

return M
