local M = {}

local bit = require("bit")
local state = require("aia.state")
local context = require("aia.context")

M.client = nil

M.connect_tcp = function()
    if M.client and M.client:is_active() then
        print('TCP client already connected')
        return
    end

    M.client = vim.uv.new_tcp()
    local ip = '10.0.0.234'
    local port = 22222
    M.client:connect(ip, port, function(err)
        if err then
            state.set_connected(false)
            state.update_status_winbar()
            return
        end
        state.set_connected(true)
        state.update_status_winbar()

        M.client:read_start(function(err, data)
            if err then
                print('Read error: ' .. err)
                M.client:close()
                M.client = nil
                return
            end
            if data then
                vim.schedule(function()
                    vim.api.nvim_exec_autocmds("User", {
                        pattern = "ServerResponse",
                        data = { response = data },
                    })
                end)
            else
                state.set_connected(false)
                state.update_status_winbar()
                M.client:close()
                M.client = nil
            end
        end)
    end)
end



local function pack_u32_be(n)
    return string.char(
        bit.band(bit.rshift(n, 24), 0xFF),
        bit.band(bit.rshift(n, 16), 0xFF),
        bit.band(bit.rshift(n, 8), 0xFF),
        bit.band(n, 0xFF)
    )
end

local function send_tcp(json)
    local len = #json
    local prefix = pack_u32_be(len)
    M.client:write(prefix .. json)
end

M.write_prompt = function(content)
    if M.client and M.client:is_active() then
        local request = {
            request_type = "prompt",
            project_id = content.project_id,
            content = content
        }
        local json_request = vim.fn.json_encode(request)
        send_tcp(json_request)
    end
end

M.write_new_project = function(projectName, content)
    if M.client and M.client:is_active() then
        local request = {
            request_type = "new_project",
            project_id = projectName,
            content = { prompt = content, harpoon_files = "", git_diff = "" }
        }
        local json_request = vim.json.encode(request)
        send_tcp(json_request)
    end
end


local function close_tcp()
    if M.client and M.client:is_active() then
        M.client:shutdown(function()
            M.client:close()
            M.client = nil
            state.set_connected(false)
            state.update_status_winbar()
        end)
    end
end

vim.api.nvim_create_autocmd('VimLeave', {
    callback = function()
        close_tcp()
    end,
})
vim.api.nvim_create_user_command('TcpConnect', M.connect_tcp, {})

return M
