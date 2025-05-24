local nvim_lsp = require("lspconfig")

nvim_lsp.clangd.setup({
	cmd = { "clangd", "--query-driver=/usr/bin/arm-none-eabi-g*" },
})
