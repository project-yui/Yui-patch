# Yukihana-Patch

适用于Telecord的底层模块。

## 功能

1. 拦截Telecord的 `index.js` 验证

## 逻辑

1. 创建一个默认的 `index.default.js`，内容未安装时的 `index.js` 内容; 而 `index.js` 则为修改的内容。
2. 拦截CreateFileW
3. 如果第一次访问 `index.js`，判定为校验文件，返回 `index.default.js` 的fd
4. 否则返回 `index.js` 的fd

## 使用

1. 构建生成 `dbghelp.dll` 文件
2. 放置于Telecord目录

### 注意：

> 如果放置于Telecord官方安装目录，会由于权限问题无法正常使用。
> 
> 但是Yukihana项目使用的Telecord程序是直接解压exe的，所以没有权限问题。(将来会变更为使用临时文件目录的方法，以支持官方安装目录)

## 声明

请勿用于非法用途

参考项目：https://github.com/LiteLoaderQQNT/QQNTFileVerifyPatch