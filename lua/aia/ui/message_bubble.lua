local M = {}

local USER_HL_GROUP = "AIAUserMessageBg"
local AI_HL_GROUP = "AIAAiMessageBg"
local PADDING_LEFT_AI = 2    -- spaces for AI message indent
local PADDING_RIGHT_USER = 2 -- spaces for user message indent from right edge of bubble

-- Helper function to wrap text manually
local function wrap_text(text, max_width)
    local wrapped_lines = {}
    local current_line = ""
    for word in text:gmatch("%S+") do -- Split by whitespace, iterate words
        if #current_line > 0 and (#current_line + 1 + #word) <= max_width then
            current_line = current_line .. " " .. word
        elseif (#current_line + #word) <= max_width then -- First word on line
            current_line = word
        else
            -- Word doesn't fit, push current_line and start new
            table.insert(wrapped_lines, current_line)
            current_line = word
        end
    end
    if #current_line > 0 then
        table.insert(wrapped_lines, current_line)
    end
    return wrapped_lines
end

--- Appends a message bubble to a Neovim buffer.
--- @param target_buf number The buffer ID to append the message to.
--- @param message_lines table A table of strings representing the message content.
--- @param msg_type string "user" or "ai".
--- @param win_width number The width of the window where the buffer is displayed, for wrapping.
function M.append_message(target_buf, message_lines, msg_type, win_width)
    local current_buf_lines = vim.api.nvim_buf_get_lines(target_buf, 0, -1, false)
    local start_row = #current_buf_lines

    -- Add some empty lines for spacing before the new bubble
    vim.api.nvim_buf_set_lines(target_buf, start_row, start_row, false, { "", "" })
    start_row = start_row + 2

    local hlg = (msg_type == "user") and USER_HL_GROUP or AI_HL_GROUP
    local formatted_message_lines = {}

    local bubble_content_width
    if msg_type == "user" then
        -- User bubble will take 60% of the window width, right-aligned
        bubble_content_width = math.floor(win_width * 0.6) - PADDING_RIGHT_USER
    else -- ai
        -- AI bubble will take 80% of the window width, left-aligned
        bubble_content_width = math.floor(win_width * 0.8) - PADDING_LEFT_AI
    end

    local full_message = table.concat(message_lines, "\n") -- Join lines for wrapping
    local lines = {}
    for line in full_message:gmatch("[^\n]+") do
        table.insert(lines, line)
    end
    table.insert(lines, "")

    vim.api.nvim_buf_set_lines(target_buf, start_row, start_row, false, lines)

    -- Apply highlight to the inserted lines
    local end_row = start_row + #formatted_message_lines

    -- Add a final blank line for spacing after the bubble
    vim.api.nvim_buf_set_lines(target_buf, end_row, end_row, false, { "" })
end

return M
