const fs = require('fs');
const digital = {};
for(let i = 0; i<14; i++){
    digital[`${i}`] = {
        IO: "O",
        component: "",
        description: "Detached"
    }
}

const analog = {};

for(i=0; i<6; i++){
    analog[`A${i}`]= {
        IO: "I",
        component: "",
        description: "Detached"
    }
};



fs.writeFileSync('samplePins.json', JSON.stringify({
    analog, digital
}, null, 4));