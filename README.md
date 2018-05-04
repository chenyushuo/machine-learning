这是一个简单的神经网络。

简介：

    ./main [option]..

描述：

    -m MODE
    --mode=MODE
        设置模式，只接受TRAINING和TESTING两种

    -i FILE
    --input=FILE
        设置读入文件

    -o FILE
    --output=FILE
        设置输出文件

    --node=val1,val2,..
        设置隐含层节点数，仅在TRAINGING时生效

    -h
    --help
        显示帮助信息

    --learning_rate=val
        设置learning rate， 默认为1.0（不能设置为非正数，仅在TRAINGING时生效）

    --epslion=val
        设置学习精度，默认为1E-9（不能设置为负数，仅在TRAINGING时生效）

    --recursion_times=val
        设置循环次数，负数表示不设限制，默认为-1（不能设置为0，仅在TRAINGING时生效）

    --block_size=val
        设置每次学习的样本个数，默认为100（不能设置为非正数，仅在TRAINGING时生效）

    运行时可按p键暂停
    暂停时可按p键继续，按q键保存并退出