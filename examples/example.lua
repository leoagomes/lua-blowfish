blowfish = require "blowfish"

-- blowfish.create
-- will allocate blowfish context linked to the key
key = blowfish.create ("an awesome password")

string = "fried potatoes make awesome fries."

-- the encrypted text
encrypted, paddingcount = key:encrypt (string)

print ("original string:\n" .. string .. "\nsize: " .. #string .. "\n")

print ("encrypted:")
print ("padding: " .. paddingcount .. "\n" .. encrypted)
print ("string size: " .. #encrypted .. "\n")

-- decrypted text.
decrypted, paddingcount = key:decrypt (encrypted)

print ("decrypted:")
print ("padding: " .. paddingcount .. "\n" .. decrypted)
print ("string size: " .. #decrypted .. "\n")
