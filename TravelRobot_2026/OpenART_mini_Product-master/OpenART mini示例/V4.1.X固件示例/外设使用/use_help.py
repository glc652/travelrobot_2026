# 导入需要查看帮助的库
import seekfree

# 使用help查看库中的对象
help(seekfree)

# 使用help查看库对象中的方法（函数）和常量（宏定义）
help(seekfree.IPS200)

# 如果库对象方法中有help方法，则可以调用此方法看到详细参数
seekfree.IPS200().help()