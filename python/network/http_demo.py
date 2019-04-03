import ssl
import urllib
from urllib import parse
from urllib import request

ssl._create_default_https_context = ssl._create_unverified_context

url = "https://www.baidu.com"

postdata = {"username":"un","password":"pd"}
data =  parse.urlencode(postdata).encode("utf-8")

user_agent = "Mozilla/4.0 (compatible;MSIE 5.5; Windows NT"
refer = "http://www.XXXXXXXXXXXx.com"

header = {"User-Agent":user_agent, "Refer":refer}

req = request.Request(url,data, header)

resp = request.urlopen(req)

print(resp.read())
