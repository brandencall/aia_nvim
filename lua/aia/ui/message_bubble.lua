local M = {}

local PADDING_LEFT_AI = 2    -- spaces for AI message indent
local PADDING_RIGHT_USER = 2 -- spaces for user message indent from right edge of bubble

local function wrap_text(text, max_width)
    local wrapped = {}
    for line in text:gmatch("[^\n]+") do
        while #line > max_width do
            local cut = max_width
            while cut > 1 and line:sub(cut, cut) ~= " " do
                cut = cut - 1
            end
            if cut == 1 then cut = max_width end
            table.insert(wrapped, vim.trim(line:sub(1, cut)))
            line = vim.trim(line:sub(cut + 1))
        end
        table.insert(wrapped, line)
    end
    return wrapped
end

--- @param lines table The message content
--- @param content_width number The width of the content that the message is in.
--- @param title string The width of the window where the buffer is displayed, for wrapping.
local function create_message_bubble(lines, content_width, title)
    local top
    local bottom

    if title ~= nil and #title > 0 then
        top    = "╭" .. " " .. title .. " " .. string.rep("─", content_width - #title) .. "╮"
        bottom = "╰" .. string.rep("─", content_width + 2) .. "╯"
    else
        top    = "╭" .. string.rep("─", content_width + 2) .. "╮"
        bottom = "╰" .. string.rep("─", content_width + 2) .. "╯"
    end

    local message = { top }
    for _, line in ipairs(lines) do
        local padding = string.rep(" ", content_width - #line)
        table.insert(message, "│ " .. line .. padding .. " │")
    end
    table.insert(message, bottom)
    return message
end

--- @param lines table The lines that make up the content.
local function get_content_width(lines)
    local content_width = 0
    for _, line in ipairs(lines) do
        content_width = math.max(content_width, #line)
    end
    return content_width
end

--- @param bubble_indent number How much to indent the message bubble.
--- @param message_bubble table How much to indent the message bubble.
local function add_bubble_indent(bubble_indent, message_bubble)
    if bubble_indent > 0 then
        for i, line in ipairs(message_bubble) do
            message_bubble[i] = string.rep(" ", bubble_indent) .. line
        end
    end
end

--- Appends a user message bubble to a Neovim buffer.
--- @param target_buf number The buffer ID to append the message to.
--- @param message_lines table A table of strings representing the message content.
--- @param win_width number The width of the window where the buffer is displayed, for wrapping.
function M.append_user_message(target_buf, message_lines, win_width)
    local bubble_content_width = math.floor(win_width * 0.4)
    local message = table.concat(message_lines, "\n")

    local lines = wrap_text(message, bubble_content_width)
    local content_width = get_content_width(lines)
    local message_bubble = create_message_bubble(lines, content_width, "YOU")

    local bubble_alignment = 5
    local bubble_indent = win_width - content_width - bubble_alignment
    add_bubble_indent(bubble_indent, message_bubble)

    vim.api.nvim_buf_set_lines(target_buf, -1, -1, false, message_bubble)
    vim.api.nvim_buf_set_lines(target_buf, -1, -1, false, { "" })
end

--- Appends an AI message bubble to a Neovim buffer.
--- @param target_buf number The buffer ID to append the message to.
--- @param message_lines table A table of strings representing the message content.
function M.append_ai_message(target_buf, message_lines)
    local line_count = vim.api.nvim_buf_line_count(target_buf)
    local last_line = vim.api.nvim_buf_get_lines(target_buf, line_count - 1, line_count, true)[1]
    if last_line == "Processing..." then
        vim.api.nvim_buf_set_lines(target_buf, line_count - 1, line_count, false, message_lines)
        vim.api.nvim_buf_set_lines(target_buf, -1, -1, false, { "" })
    else
        vim.api.nvim_buf_set_lines(target_buf, -1, -1, false, message_lines)
    end
end

return M
