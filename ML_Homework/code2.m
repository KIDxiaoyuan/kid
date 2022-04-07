clear;

global num_of_dots;
global dots_x;
global dots_y;
num_of_dots = 12;

dots_x = [0 1.5 1.5 -2 -4 -5 2    4    5 -2 -3 3 -1.5 -2.5 0.1];
dots_y = [0 10  12  11 -8 2  -1.5 -2.5 1 -2  8 6    0    0 0.2];

draw_dist(), hold on;
scatter(dots_x, dots_y, 'MarkerFaceColor','blue');


x_min = min(dots_x);
y_min = min(dots_y);
x_max = max(dots_x);
y_max = max(dots_y);
x_range = x_max - x_min;
y_range = y_max - y_min;

tmp = 1000;
tmp_max = tmp;
tmp_min = 0.001;
alpha = 0.98;

x_old = rand() * x_range + x_min;
y_old = rand() * y_range + y_min;
x_new = x_old;
y_new = y_old;
s_old = dist(x_old, y_old);
s_new = s_old;

txttt = text(0, 0, 'Init');


while(tmp > tmp_min)

  %因为是在二维平面里，所以产生两个维度的随机扰动
  delta1 = (rand - 0.5);
  delta2 = (rand - 0.5);
  x_new = delta1 + x_old;
  y_new = delta2 + y_old;
    
  if(x_new > x_max || x_new < x_min)
    x_new = x_new - 2 * delta1;
  end
  if(y_new > y_max || y_new < y_min)
    y_new = y_new - 2 * delta2;
  end
    
  s_new = dist(x_new, y_new);
    
  dE = s_new - s_old;
  j = judge(dE, tmp);
    
  if(j)
    s_old = s_new;
    x_old = x_new;
    y_old = y_new;
  end
    
  delete(txttt);
  txttt = text(-4, 6, {['Dist: ',num2str(s_old)];['Tmp: ', num2str(tmp)]});
  hold on, scatter(x_old, y_old, '.'), pause(0.01);
    
  tmp = tmp * alpha;

end

text(x_old, y_old, ['Solve: ', num2str(s_old)]);


%计算点到点集的距离
function [d] = dist(x, y)
    global num_of_dots;
    global dots_x;
    global dots_y;
    
    d = 0;
    for ii = 1:num_of_dots
        d = d + sqrt((x - dots_x(ii))^2 + (y - dots_y(ii))^2);
    end
end

function draw_dist()
    global dots_x;
    global dots_y;
    
    x_min = floor(min(dots_x));
    y_min = floor(min(dots_y));
    x_max = ceil(max(dots_x));
    y_max = ceil(max(dots_y));
    
    resol = 10;
    
    dx = x_min: 1/resol : x_max;
    dy = y_min: 1/resol : y_max;
    
    [xx, yy] = meshgrid(dx, dy);
    zz = zeros(length(dy), length(dx));

    for ii = 1:length(dx)
        for jj = 1:length(dy)
            zz(jj, ii) = dist(dx(ii), dy(jj));
        end
    end
    
    contour(xx, yy, zz);
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
