const fs = require('fs')
const path = require('path')

const p = path.resolve(__dirname, '../vcpkg-configuration.json')
const cfg = JSON.parse(fs.readFileSync(p).toString())
cfg['default-registry'].repository = 'https://github.com/msojocs/vcpkg'
cfg['default-registry'].baseline = '47a484f679d92ce89f551d0e026adf291b9fc336'

fs.writeFileSync(p, JSON.stringify(cfg, null, 4))
