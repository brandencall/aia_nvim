local M = {}

local harpoon = require("harpoon")

M.get_harpoon_context = function()
    local harpoon_signatures = M.get_harpoon_function_signatures()
    local result = {}
    for key, value in pairs(harpoon_signatures) do
        table.insert(result, { file = key, functions = value })
    end
    return result
end

M.get_harpoon_function_signatures = function()
    local list = harpoon:list()
    local result = {}
    for _, item in ipairs(list.items) do
        local bufnr = vim.fn.bufadd(item.value)
        local signatures = M.get_function_signatures(bufnr)
        result[item.value] = signatures
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

vim.api.nvim_create_user_command("TreesitterTest", M.get_function_signatures, { desc = "test" })
vim.api.nvim_create_user_command("ListFiles", M.get_harpoon_context, { desc = "test" })

return M
