let users = [];
let pairedUsers = [];


function isEmpty(obj) {
    for (const prop in obj) {
      if (Object.hasOwn(obj, prop)) {
        return false;
      }
    }
  
    return true;
}

function filterArrOfObjByValue(arrayOfObject, term) {
    var ans = arrayOfObject.filter(function(v,i) {
        if(v.mobile_user.name.toLowerCase().indexOf(term) >=0 || v.pc_user.name.toLowerCase().indexOf(term) >=0) {
            return true;
        } else false;
    });
    return ans;
}

function filterArrOfObjRemByIdValue(id) {
    //return [...new Map(arr.map(item => [item[key], item])).values()];
    for(let i = 0; i < pairedUsers.length; i++)
    {
        if(pairedUsers[i]?.mobile_user?.id === id)
        {
            delete pairedUsers[i]?.mobile_user
        }
        if(pairedUsers[i]?.pc_user?.id === id)
        {
            delete pairedUsers[i]?.pc_user
        }
    }
    return [...pairedUsers];
}
const addUser = ({ id, name, token, iss}) => {
    name = name.trim().toLowerCase();

    const existingUser = users.find((user) => user.name === name && user.iss === iss);

    if(existingUser) {
        return { error: 'Username is taken'};
    }

    const user = { id, name, token, iss};
    
    users.push(user);

    return { user };
}

const removeUser = (id) => {
    const index = users.findIndex((user) => user.id === id);

    if(index !== -1) {
        return users.splice(index, 1)[0]
    }
}

const removePairedUser = (id) => {
    let pU = filterArrOfObjRemByIdValue(id);
    if(isEmpty(pU)){
        console.log('There is not paired users right now.')
        return ;
    }
    return pU;
}

const getUser = (id) => users.find((user) => user.id === id);

const getUsers = () => users;

const pairUsers = (name) => {
    const pair_users = [...users];
    const mobile_user = pair_users.find((user) => user.name === name && user.iss == "mobile");
    const pc_user = pair_users.find((user) => user.name === name && user.iss == "pc");
    
    
    if(mobile_user == 'undefined'){
        console.log('Mobile user not connected yet.');
    }else{
        console.log('Mobile user: ' + JSON.stringify(mobile_user));
    }

    if(pc_user == 'undefined'){
        console.log('Mobile user not connected yet.');
    }else{
        console.log('PC User:' , JSON.stringify(pc_user));
    }

    if(isEmpty(mobile_user) || isEmpty(pc_user)){
        console.log('There is not paired users right now.');
        return;
    }
    pairedUsers = [{mobile_user, pc_user}];
    console.log('Users has been paired');
    console.log(pairedUsers);
}

let areUsersPaired = () => {
    const mobile_user = pair_users.find((user) => user.name === name && user.iss == "mobile");
    const pc_user = pair_users.find((user) => user.name === name && user.iss == "pc");
    if(!isEmpty(mobile_user) && !isEmpty(pc_user)){
        console.log('Paired check: paired');
        return true;
    } else{
        return false;
    }
}

const getPairedUsersByName = (name) => {
    let pU = filterArrOfObjByValue(pairedUsers, name);
    if(isEmpty(pairedUsers)){
        console.log('There is not paired users right now.')
        return ;
    }
    return pU;
}

const getPairedUsers = () => pairedUsers;

module.exports = { addUser, removeUser, getUser, getUsers, pairUsers, getPairedUsersByName, getPairedUsers, removePairedUser, areUsersPaired};