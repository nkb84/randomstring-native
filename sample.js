const generator = require('./build/Release/randomstring-native.node');

console.time('ABC')
let randomstrings = generator.generate({
                'charset': '0123456789abcdefghijklmnopqrstuvxyz',
                'length': 10,
                'count': 1000000,
                'excludes': ['003fy23h5m', '003g18hjv7']
    })

console.timeEnd('ABC')
console.log(randomstrings.length)