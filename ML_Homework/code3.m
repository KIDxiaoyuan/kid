clear;

cities1=[0.9695,0.6606,0.5906,0.2124,0.0398,0.1367,0.9536,0.6091,0.8767,0.8148,0.3876,0.7041,0.0213,0.3429,0.7471,0.4606,0.7695,0.5006,0.3124,0.0098,0.3637,0.5336,0.2091,0.4767,0.4148,0.5876,0.6041,0.3213,0.6429,0.7471;
         0.6740,0.9500,0.5029,0.8274,0.9697,0.5979,0.2184,0.7148,0.2395,0.2867,0.8200,0.3296,0.1649,0.3025,0.8192,0.6500,0.7420,0.0229,0.7274,0.4697,0.0979,0.2684,0.7948,0.4395,0.8867,0.3200,0.5296,0.3649,0.7025,0.9192];

tmp = 1e3;
tmp_min = 1e-5;
alpha = 0.99;
s_old = dist(cities1);
cit_new = cities1;
s_new = s_old;

txttt = text(0, 0, 'Init');

counter = 0;
while(tmp > tmp_min)
  
  %产生随机扰动，对于TSP问题，随机扰动一般为交换几座城市的顺序
  cit_new = new_solve(cities1);
  s_new = dist(cit_new);
  dE = (s_new - s_old) * 500;
  j = judge(dE, tmp);

  if(j)
    cities1 = cit_new;
    s_old = s_new;
  end
    
  if(dE < 0)
    counter = 0;
    %降温
    tmp = tmp * alpha;
    %绘图
    clf;
    text(0.8, 0.9, {['tmp: ', num2str(tmp)];['dist: ', num2str(s_old)]});
    hold on;
    draw_route(cities1);
    pause(0.01);
  else
    counter =counter + 1;
  end

  if(counter > 1e4)
    break;
  end
end


function draw_route(in)
  plot(in(1,:), in(2,:),'-o');
end

function [d] = dist(in)
  num = size(in, 2) - 1;
  d = 0;
  for ii = 1: num
    d = d + sqrt(sum((in(:, ii+1) - in(:, ii)).^2));
  end
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
function [cities2] = new_solve(in)
  cities2 = in;
  num_of_cities = size(cities2,2);
    
  while 1
    cit1 = ceil(rand() * num_of_cities);
    cit2 = ceil(rand() * num_of_cities);
    if(cit1 ~= cit2)
      break;
    end
  end
    
  tmp = cities2(:, cit1);
  cities2(:, cit1) = cities2(:, cit2);
  cities2(:, cit2) = tmp;
end
