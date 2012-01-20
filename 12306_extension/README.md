12306 订票助手 1.3.10
===========================

BUG提交或者查看已经解决的问题： https://github.com/zzdhidden/12306/issues

功能
------------

*	支付方式需网银支持google浏览器支付，目前成功使用招商银行手机支付购票成功
*	自动尝试登录，登录成功提示
*	根据条件自动查票，有票以后声音提示
*	订单自动重试


使用
------------

###安装(谷歌浏览器版) 推荐

有使用 12306自动登录 和 12306自动查询 的同学可在 插件管理(chrome://settings/extensions) 里面卸载了，因为功能重复了

使用谷歌浏览器 [点击这里](https://github.com/zzdhidden/12306/raw/master/12306BookingAssistant.user.js), 然后确认安装此UserScript


###安装(360极速浏览器)

移步[火车票刷票机](http://chrome.360.cn/extensions/detail/jaojfpnikjjobngdfbgbhflfooidihmg/)


###安装(IE版) 

首先安装 [Trixie](http://www.bhelpuri.net/Trixie/TrixieDownload.htm)

下载[这个脚本](https://github.com/zzdhidden/12306/raw/master/12306BookingAssistant.user.js) 保存到 C:\Program Files\Bhelpuri\Trixie\Scripts文件夹下（64位系统在：C:\Program Files (x86)\Bhelpuri\Trixie\Scripts）

在工具->Trixie Options（Trixie选项）中点击Reload Scripts（重新载入）按钮，再选中该脚本，即可使用（如在工具里面没发现Trixie Options请重启浏览器）。

如果发现乱码，请将脚本保存为UTF-8编码，然后再重新在Trixie Options选项中Reload脚本。


###安装(火狐浏览器版本) 

首先安装 [Greasemonkey](https://addons.mozilla.org/zh-CN/firefox/addon/greasemonkey/) 插件来支持UserScript
然后 [点击这里](https://github.com/zzdhidden/12306/raw/master/12306BookingAssistant.user.js), 然后确认安装


###开始订票

安装成功后打开 [https://dynamic.12306.cn/otsweb/](https://dynamic.12306.cn/otsweb/) 开始订票吧

###登录

首先输入好登录名，密码，和验证码。 点击自动登录，即可进行多次尝试登录，直至登录成功

![登录](https://github.com/zzdhidden/12306/raw/master/login.jpg)

###刷票

登录之后到车票预订页面，选择好出发地，目的地，和出发时间，点击开始刷票，就会不断更新火车票，只到有票为止，刷到之后会有通知提示和声音提示

![查询](https://github.com/zzdhidden/12306/raw/master/query.jpg)

###订单

当跳转到订单页时，如果没有选择用户会自动选择第一个用户，需要用户自己输入验证码
自动提交订单功能会在用户过多等预订失败的情况下，自动重新提交订单

![订单](https://github.com/zzdhidden/12306/raw/master/order.jpg)

那些贡献者们....
------------

*	[Jingqin Lynn](https://gist.github.com/quietlynn) [自动查票](https://gist.github.com/1554666) 
*	[Kevintop](https://gist.github.com/kevintop) [自动登录](https://gist.github.com/1570973) [自动订单](https://gist.github.com/1577671) 
*	[Luobailiang](https://github.com/luobailiang) [增加了提交频率和停止等功能](https://gist.github.com/1577671#gistcomment-74361)

版本历史
-----------------------

###1.3.10
*	登录2秒间隔

###1.3.9

*	出现“未知错误”继续登录
*	出现“没有足够的票”继续提交订单


###1.3.8

*	修正BUG: IE刷票不显示


###1.3.7

*	增加改签中刷票功能
*	修正学生票不能刷的BUG
*	车票预订成功声音提示


###1.3.6

*	解决自动登录中密码输入错误会锁定账号的BUG

###1.3.5

*	解决Firefox自动订单失败

###1.3.4

*	取消系别初始化和订单时间修改对订单提交的影响


###1.3.3

*	限定多个车次刷票 [#29](https://github.com/zzdhidden/12306/issues/29)
*	Firefox greasemonkey插件支持 [#21](https://github.com/zzdhidden/12306/issues/21)
*	更新自动提交代码 [#25](https://github.com/zzdhidden/12306/issues/25)

###1.3.2

*	改变刷票页面字体高亮颜色 [#14](https://github.com/zzdhidden/12306/issues/14)
*	增加提交频率和停止提交订单功能 [#13](https://github.com/zzdhidden/12306/issues/13)
*	订单中日期修改 [#12](https://github.com/zzdhidden/12306/issues/12)


###1.3.1

*	解决点击“我的12306”会不断跳转BUG

###1.3

*	修正自动登录失败的BUG
*	IE支持


###1.2

*	订单页面自动选择第一个用户
*	订单自动重试

###1.1

*	在登录之后，登录页自动跳转到查票页面
*	修正BUG:没有桌面提示的情况，刷票成功，音乐无法播放
*	增加刷特定票种帮助信息

###1.0

*	自动登录
*	自动查票

获取更新
--------------------

*	GitHub关注： https://github.com/zzdhidden/12306

