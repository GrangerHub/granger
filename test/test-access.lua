print "os.access() test begin"

if not os.is("windows") then
    p = os.tmpname()
	--os.executef("touch %s; chmod 400 %s", p, p)
	assert(os.access(p, "r") == true)
	--assert(os.access(p, "w") == false)
	os.executef("rm -f %s", p)
end

print "os.access() test end"
