clear;

%范围
section_l = 0;
section_h = 9;

%绘制函数图像
draw_base();

%初始温度，停止温度与降温系数
tmp = 1e5;
tmp_min = 1e-3;
alpha = 0.98;

%生成初始随机解
x_old = (section_h - section_l) * rand() + section_l;
x_new = x_old;
s_old = val(x_old);
s_new = s_old;

text_lt = text(0, 0, 'Init');

%计数器
counter = 0;

%退火的主体部分，一个循环
while(tmp > tmp_min)
  %随机扰动
  delta = (rand() - 0.5) * 3;
  x_new = x_old + delta;
  %扰动的值小于一半的区间范围时，可以用这种办法防止新值超出区间范围
  if(x_new < section_l || x_new > section_h)
    x_new = x_new - 2 * delta;
  end
 
  s_new = val(x_new);

  %求差值，这里是找最大值而非最小值，所以不是s_new - s_old
  dE = s_old - s_new;
        
  %判断
  j = judge(dE, tmp);
  if(j)
    s_old = s_new;
    x_old = x_new;
  end
          
  %只有当dE < 0的情况下才降温
  if(dE < 0)
    delete(text_lt);
    hold on, scatter(x_old, s_old);
    hold on, text_lt = text(0.3, 21, ['tmp: ', num2str(tmp)]);
    pause(0.01);
    %上面是绘图的代码，下面降温
    tmp = tmp * alpha;
  else
    counter = counter + 1;
  end
            
  %当接受更差的解的概率太小时，若又长时间找不到更优的解，那么退出循环，结束算法
  if(counter > 10000)
    break;
  end
    
end

function [y] = val(x)
    y = x + 10 * sin(5 * x) + 7 * cos(4 * x);
end
function [y] = judge(dE, t)
  if(dE < 0)
    y = 1;
  else
    d = exp(-(dE / t));
    if(d > rand)
      y = 1;
    else
      y = 0;
    end
  end
end
function draw_base()
    X = 0: 0.01:9;
    M = val(X);
    plot(X, M);
end
