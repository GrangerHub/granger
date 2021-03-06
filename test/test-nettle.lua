--
-- test-nettle.lua
-- test cases for nettle library
-- Copyright (c) 2016 Jeff Kent <jeff@jkent.net>
--

print "nettle tests begin"

empty_hash = tostring(nettle.sha256())

ctx = nettle.sha256()
ctx:update(nil)
assert(empty_hash == tostring(ctx))

ctx = nettle.sha256()
ctx:update("Hello World!")
ctx:update("Hello World!")
hash = tostring(ctx)
assert(hash == "95a5a79bf6218dd0938950acb61bca24d5809172fe6cfd7f1af4b059449e52f8")

ctx = nettle.sha256()
ctx:update("Hello World!Hello World!")
hash = tostring(ctx)
assert(hash == "95a5a79bf6218dd0938950acb61bca24d5809172fe6cfd7f1af4b059449e52f8")

require "util"
hash = md5_file("../COPYING")
assert(hash == "b234ee4d69f5fce4486a80fdaf4a4263")

hash = sha256_file("../COPYING")
assert(hash == "8177f97513213526df2cf6184d8ff986c675afb514d4e68a404010521b880643")

print "nettle tests completed"
