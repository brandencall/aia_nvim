local M = {}

local tcp = require("aia.tcp_client");

vim.api.nvim_create_user_command("CreateProject", function(opts)
    local project_root = vim.fs.root(0, ".git")
    if project_root == nil then
        print("The project needs to be a .git directory")
        return
    end
    local context = table.concat(opts.fargs, " ")
    tcp.write_new_project(project_root, context)
end, {
    nargs = "*", -- zero or more args, space-separated
    desc = "Creates a project on the server and adds context to the project",
})

return M
