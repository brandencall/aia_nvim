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

    local formatted_message_lines = {}

    local bubble_content_width
    if msg_type == "user" then
        -- User bubble will take 60% of the window width, right-aligned
        bubble_content_width = math.floor(win_width * 0.6)
    else -- ai
        -- AI bubble will take 80% of the window width, left-aligned
        bubble_content_width = math.floor(win_width * 0.8)
    end

    local message = table.concat(message_lines, "\n")

    local lines = wrap_text(message, bubble_content_width)

    -- Measure true width
    local content_width = 0
    for _, line in ipairs(lines) do
        content_width = math.max(content_width, #line)
    end

    local top      = "╭" .. string.rep("─", content_width + 2) .. "╮"
    local bottom   = "╰" .. string.rep("─", content_width + 2) .. "╯"

    local bordered = { top }
    for _, line in ipairs(lines) do
        local padding = string.rep(" ", content_width - #line)
        table.insert(bordered, "│ " .. line .. padding .. " │")
    end
    table.insert(bordered, bottom)

    -- Add left indent
    --if indent > 0 then
    --    for i, line in ipairs(bordered) do
    --        bordered[i] = string.rep(" ", indent) .. line
    --    end
    --end

    -- Insert into buffer at end
    vim.api.nvim_buf_set_lines(target_buf, -1, -1, false, bordered)

    --local full_message = table.concat(message_lines, "\n") -- Join lines for wrapping
    --local lines = {}
    --for line in full_message:gmatch("[^\n]+") do
    --    table.insert(lines, line)
    --end
    --table.insert(lines, "")

    --vim.api.nvim_buf_set_lines(target_buf, start_row, start_row, false, lines)

    -- Apply highlight to the inserted lines
    local end_row = start_row + #formatted_message_lines

    -- Add a final blank line for spacing after the bubble
    --vim.api.nvim_buf_set_lines(target_buf, end_row, end_row, false, { "" })
end

return M
