# EasyCppTools

[English](README.md) | [简体中文](README_CN.md)

EasyCppTools顾名思义，就是一个简单的C++工具集，该集合计划包含轻巧Json解析器，轻巧加/解密集合，在线API接口集合。每天持续更新中，希望您参与到其中来(偶尔一两天会摆烂，见谅)。

## 功能及开发状态

| 功能 | 状态 | 备注 |
| :---: | :---: | :--- |
| 数组输出 | 完成 | 无 |
| 对象输出 | 完成 | 无 |
| Json 解析器 | 完成 | 完成文件测试。 |
| 加密 | 正在进行 | 无 |
| 解密 | 取消 | 部分加密方法不适合解密。 |
| 高级数据排序 | 计划(长周期) | 数据处理 |
| - | 计划 | 无 |

## 如何使用

``` sh
cd <EasyCppTools_root_dir>
mkdir -p build && cd build
cmake .. && make
```

修改了生成方案，从动态库文件到可执行文件。

## 开源许可

[Apache License 2.0](LICENSE)