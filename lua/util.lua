--
-- util.lua
-- various utility functions
-- Copyright (c) 2015 Jeff Kent <jeff@jkent.net>
--

function hash_file(file)
	f = io.open(file, "r")
	if f == nil then
		return nil
	end
	ctx = nettle.sha256()
	repeat
		buf = f:read(0x10000)
		ctx:update(buf)
	until buf == nil
	f:close()
	return tostring(ctx)
end
