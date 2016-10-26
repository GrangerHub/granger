function hash_file(file)
	f = io.open(file, "r")
	ctx = nettle.sha256()
	repeat
		buf = f:read(0x10000)
		ctx:update(buf)
	until buf == nil
	return tostring(ctx)
end
