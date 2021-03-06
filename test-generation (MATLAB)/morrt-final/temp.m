%for i=1:3000, 
%    rrt_sample_t(i, 16) = tree(i).t;
%end

%i=4;
%t=9.8e-4;
%x=floor( t/0.1e-4) +2
%if(x>100)
%    x=100;
%out = rrt_sample_t(x, i)
%tree(x).y(i)

%compute deviation
%for i=1:3000, 
%    for j=1:15;
%        t = rrt_sample_t(i, 16);
%        deviation(i, j) = rrt_sample(i,j) - getNominalValue(rrt_sample_t, j,t);
%    end
%end
%deviation_t = zeros(3000, 16);
%deviation_t(:, 1:15) = deviation;
%deviation_t(:, 16) = rrt_sample_t(:, 16);

%min = zeros(100, 16);
%max = zeros(100, 16);
%max(:, 16) = rrt_sample_t(1:100, 16);
%min(:, 16) = rrt_sample_t(1:100, 16);
for i=1:100, 
    t=rrt_sample_t(i, 16);
    
    for j=1:15,
        minIndex=i;
        minn=rrt_sample_t(i, j);
        maxIndex=i;
        maxx=rrt_sample_t(i, j);
        
        for ii=1:3000,
            if( t==rrt_sample_t(ii, 16) )
                if(rrt_sample_t(ii, j)<minn )
                minIndex=ii;
                minn=rrt_sample_t(ii, j);
            end
            
            if( rrt_sample_t(ii, j)>maxx )
                maxIndex=ii;
                maxx=rrt_sample_t(ii, j);
            end
            end
        end
        min(i, j)=minn;
        max(i,j)=maxx;
    end
    
end