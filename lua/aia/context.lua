local M = {}

local harpoon = require("harpoon")

local function get_harpoon_function_signatures()
    local list = harpoon:list()
    local result = {}
    for _, item in ipairs(list.items) do
        local bufnr = vim.fn.bufadd(item.value)
        local signatures = M.get_function_signatures(bufnr)
        result[item.value] = signatures
    end
    return result
end

M.get_project_context = function(prompt)
    local project_id = vim.fs.root(0, ".git")
    if project_id == nil then
        project_id = ""
    end
    local harpoon_context = M.get_harpoon_context()
    local git_diff = M.get_git_diff()
    -- local file_structure = M.get_project_tree(project_id)
    local request = {
        prompt = prompt,
        project_id = project_id,
        harpoon_files = harpoon_context,
        git_diff = git_diff,
        file_structure = ""
    }
    return request
end

M.get_git_diff = function()
    local max_chars = 4000
    local context_lines = 3
    local cmd = "git diff --no-ext-diff --text -U" .. tostring(context_lines)
    local git_diff = vim.fn.system(cmd)

    if vim.v.shell_error ~= 0 then
        return ""
    end

    if #git_diff > max_chars then
        git_diff = string.sub(git_diff, 1, max_chars) .. "\n... [git diff truncated]"
    end

    return git_diff
end

M.get_harpoon_context = function()
    local harpoon_signatures = get_harpoon_function_signatures()
    local result = {}
    for key, value in pairs(harpoon_signatures) do
        table.insert(result, { file = key, functions = value })
    end
    return result
end


M.get_function_signatures = function(bufnr)
    vim.fn.bufload(bufnr)
    local filetype = vim.api.nvim_buf_get_option(bufnr, "filetype")

    local filetype_to_parser = {
        cpp = "cpp",
        cs = "c_sharp",
        lua = "lua",
    }
    local parser_name = filetype_to_parser[filetype]
    if not parser_name then
        vim.notify("Tree-sitter: Unsupported filetype: " .. filetype, vim.log.levels.WARN)
        return {}
    end

    local ok, parser = pcall(require('nvim-treesitter.parsers').get_parser, bufnr, parser_name)
    if not ok or not parser then
        vim.notify("Tree-sitter: Parser not found for " .. parser_name, vim.log.levels.WARN)
        return {}
    end

    local tree = parser:parse()[1]
    if not tree then
        vim.notify("Tree-sitter: Failed to parse buffer", vim.log.levels.WARN)
        return {}
    end

    local query_name = "functions"
    local ok, query = pcall(vim.treesitter.query.get, parser_name, query_name)
    if not ok or not query then
        vim.notify(
            "Tree-sitter: Query 'functions' not found for " ..
            parser_name .. ". Please create queries/" .. parser_name .. "/functions.scm", vim.log.levels.ERROR)
        return {}
    end

    local signatures = {}
    local root = tree:root()
    for id, node in query:iter_captures(root, bufnr, 0, -1) do
        local name = query.captures[id]
        local text = vim.treesitter.get_node_text(node, bufnr)

        if not signatures[name] then
            signatures[name] = {}
        end
        table.insert(signatures[name], text)
    end

    local count = math.max(
        #(signatures["function.name"] or {}),
        #(signatures["function.parameters"] or {}),
        #(signatures["function.qualified"] or {})
    )

    local result = {}

    for i = 1, count do
        local name
        if signatures["function.name"] and signatures["function.name"][i] then
            name = signatures["function.name"][i]
        elseif signatures["function.qualified"] and signatures["function.qualified"][i] then
            name = signatures["function.qualified"][i]
        else
            name = "<anonymous>"
        end

        local params = (signatures["function.parameters"] and signatures["function.parameters"][i]) or "()"
        table.insert(result, name .. params)
    end
    return result
end

M.get_project_tree = function(project_dir)
    project_dir = project_dir or vim.fs.root(0, ".git")
    if project_dir == nil or project_dir == "" then
        return ""
    end
    local handle = io.popen("LANG=C.UTF-8 tree -I '.git|node_modules|target|venv|__pycache__|bin' -L 3 -F --noreport " ..
        vim.fn.shellescape(project_dir))
    if not handle then return "" end
    local result = handle:read("*a")
    handle:close()
    return result
end

vim.api.nvim_create_user_command("TestTree", M.get_project_tree, { desc = "test" })
vim.api.nvim_create_user_command("ListFiles", M.get_harpoon_context, { desc = "test" })

return M
