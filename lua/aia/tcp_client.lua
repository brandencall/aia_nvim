local M = {}

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
            print('TCP error: ' .. err)
            return
        end
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
                M.client:close()
                M.client = nil
            end
        end)
    end)
end

M.write_prompt = function(prompt)
    if M.client and M.client:is_active() then
        local project_id = vim.fs.root(0, ".git")
        if project_id == nil then
            project_id = ""
        end
        local request = {
            request_type = "prompt",
            project_id = project_id,
            content = prompt
        }
        local json_request = vim.fn.json_encode(request)
        M.client:write(json_request)
    end
end

M.write_new_project = function(projectName, context)
    if M.client and M.client:is_active() then
        local request = {
            request_type = "new_project",
            project_id = projectName,
            content = context
        }
        local json_request = vim.fn.json_encode(request)
        M.client:write(json_request)
    end
end

local function close_tcp()
    if M.client and M.client:is_active() then
        M.client:shutdown(function()
            M.client:close()
            M.client = nil
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
