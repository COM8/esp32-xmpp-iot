import base64

data = u"\0test1\0password"

# Standard Base64 Encoding
encodedBytes = base64.b64encode(data.encode("utf-8"))
encodedStr = encodedBytes.decode("utf-8")

print(encodedStr)
