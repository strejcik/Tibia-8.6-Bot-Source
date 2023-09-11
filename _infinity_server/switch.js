const uuid = require('uuid');
var jwt = require('jsonwebtoken');
const { addUser, removeUser, getUser, getUsers, pairUsers, getPairedUsersByName, removePairedUser, areUsersPaired} = require('./users');

let char_details = [];
function request_switch(request, socket)
{
    let res = Buffer.from(request).toString();
    var token = res.split('?');
    let key = token[0];
    switch(key) {
        case 'pc-token':
            {
                let r = Buffer.from(request).toString();
                var t = r.split('?');
                let tokenValue = t[1];
                let token = [...Buffer.from(tokenValue).toString()];
                console.log('TOKEN:', token)
                token = token.join("").replaceAll(" ", "")
                token = token.replace(/(\r\n|\n|\r)/gm, "");
                const payload = jwt.verify(token, "secret", function(err, decoded)
                {
                if (err){
                    console.log(err);
                    socket.write('unauthorized');
                }
                else
                {
                    // socket.emit("pc-toggle-xray", "1");
                    const { error, user } = addUser({ id: uuid.v4(), name: decoded.name, token: token, iss: decoded.iss});
                    if(error){
                    return socket.write('unauthorized');
                    }
                    pairUsers(decoded.name);
                    socket.write("authorized");
                    getPairedUsersByName(decoded.name);
                }
                });
            }
          break;
          case 'mobile-token':
            {
                let r = Buffer.from(request).toString();
                var t = r.split('?');
                let tokenValue = t[1];
                let token = [...Buffer.from(tokenValue).toString()];
                console.log('TOKEN:', token)
                token = token.join("").replaceAll(" ", "")
                token = token.replace(/(\r\n|\n|\r)/gm, "");
                const payload = jwt.verify(token, "secret", function(err, decoded)
                {
                if (err){
                    console.log(err);
                    socket.write('unauthorized',);
                }
                else
                {
                    // socket.emit("pc-toggle-xray", "1");
                    const { error, user } = addUser({ id: uuid.v4(), name: decoded.name, token: token, iss: decoded.iss});
                    if(error){
                    return socket.write('unauthorized');
                    }
                    pairUsers(decoded.name);
                    socket.write("authorized");
                    getPairedUsersByName(decoded.name);
                }
                });
            }
            break;
           case 'character-details-from-pc':
            {
                let r = Buffer.from(request).toString();
                var t = r.split('?');
                let x = t[1].replace(/\s+/g, ',');
                char_details[0] = x;
                console.log(char_details);
                if(char_details.length > 10) char_details.length = 1;
                //socket.write('x');
            }
            break;
            case 'character-details-to-mobile':
            {
                if(char_details.length == 0)
                {
                    socket.write("");
                }
                else
                {
                    socket.write(char_details[0]);
                }
            }
            break;
        default:
          console.log(`Message received but doesn't match any case.`);
      }
}

module.exports = {request_switch};