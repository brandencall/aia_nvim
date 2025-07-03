local MessageBubble = {}
MessageBubble.__index = MessageBubble

function MessageBubble:new(opts)
    self = setmetatable({}, MessageBubble)

    self.lines = opts.lines or { "" }
    self.type = opts.type or "user"
    self.width = opts.width or 40
    self.row = opts.row or 0
    self.col = opts.col or 0
    self.relative = opts.relative
    self.win = opts.win

    self.height = math.max(1, #self.lines)

    self.buf = vim.api.nvim_create_buf(false, true)
    vim.api.nvim_buf_set_lines(self.buf, 0, -1, false, self.lines)

    self.win = vim.api.nvim_open_win(self.buf, false, {
        relative = self.relative,
        win = self.win,
        width = self.width,
        height = self.height,
        row = self.row,
        col = self.col,
        style = "minimal",
        border = "rounded",
        title = self.type == "user" and " You " or " BOT ",
        title_pos = "left",
    })

    return self
end

return MessageBubble
