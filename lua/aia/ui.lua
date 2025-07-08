local M = {}

local state = {
    content_win = nil,
    content_buf = nil,
    prompt_win = nil,
    prompt_buf = nil
}

M.side_win = function()
    M.create_side_win()
    M.setup_auto_close()
    M.on_submit()
end


M.create_side_win = function()
    state.content_buf = vim.api.nvim_create_buf(false, true)
    local width = math.floor(vim.o.columns * 0.5)
    local height = math.floor(vim.o.lines * 0.8)
    local content_opts = {
        split = "right",
        width = width,
        height = height,
    }
    state.content_win = vim.api.nvim_open_win(state.content_buf, true, content_opts)
    vim.api.nvim_buf_set_lines(state.content_buf, 0, -1, false, {
        "This is the content buffer.",
        "You can fill it with search results or text.",
        "",
    })
    vim.api.nvim_set_option_value("wrap", true, { win = state.content_win })
    vim.api.nvim_set_option_value("linebreak", true, { win = state.content_win })
    vim.api.nvim_set_option_value("breakindent", true, { win = state.content_win })

    state.prompt_buf = vim.api.nvim_create_buf(false, true)
    local prompt_opts = {
        split = "below",
        width = width,
        height = vim.o.lines - height,
    }
    state.prompt_win = vim.api.nvim_open_win(state.prompt_buf, true, prompt_opts)
    vim.api.nvim_buf_set_lines(state.prompt_buf, 0, -1, false, { "Prompt: " })
    vim.api.nvim_set_option_value("wrap", true, { win = state.prompt_win })
    vim.api.nvim_set_option_value("linebreak", true, { win = state.prompt_win })
    vim.api.nvim_set_option_value("breakindent", true, { win = state.prompt_win })

    vim.api.nvim_win_set_cursor(state.prompt_win, { 1, 8 }) -- After "Prompt: "
    vim.cmd("startinsert")
end

M.set_content_text = function(content)
    if state.content_buf == nil then
        vim.notify(content)
        return
    end
    local last_line = vim.api.nvim_buf_line_count(state.content_buf)
    if (content == "Processing...") then
        vim.api.nvim_buf_set_lines(state.content_buf, last_line, -1, false, { content })
    else
        local result = "BOT: " .. content
        local lines = {}
        for line in result:gmatch("[^\n]+") do
            table.insert(lines, line)
        end
        table.insert(lines, "")
        vim.api.nvim_buf_set_lines(state.content_buf, last_line - 1, -1, false, lines)
        vim.api.nvim_buf_set_option(state.content_buf, 'filetype', 'markdown')
    end
end

M.set_user_content = function(prompt)
    local last_line = vim.api.nvim_buf_line_count(state.content_buf)
    local content = "You: " .. prompt
    vim.api.nvim_buf_set_lines(state.content_buf, last_line, -1, false, { content })
end

M.close_windows = function()
    if vim.api.nvim_win_is_valid(state.content_win) then
        vim.api.nvim_win_close(state.content_win, true)
    end
    if vim.api.nvim_win_is_valid(state.prompt_win) then
        vim.api.nvim_win_close(state.prompt_win, true)
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
        pattern = { tostring(state.content_win), tostring(state.prompt_win) },
        callback = M.close_windows,
        once = true, -- Run only once to avoid repeated triggers
    })
end

M.on_submit = function()
    vim.keymap.set("i", "<CR>", function()
        local lines = vim.api.nvim_buf_get_lines(state.prompt_buf, 0, -1, false)
        local input = table.concat(lines, "\n"):sub(8) --Prompt prefix removed

        vim.schedule(function()
            M.set_user_content(input)
        end)
        vim.schedule(function()
            vim.api.nvim_buf_set_lines(state.prompt_buf, 0, -1, false, { "Prompt:" })
        end)

        vim.api.nvim_exec_autocmds("User", {
            pattern = "OnPromptSubmit",
            data = { input = input },
        })
    end, { buffer = state.prompt_buf, noremap = true, silent = true, expr = true })
end

return M
