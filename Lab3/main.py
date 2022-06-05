import random
import heapq
import time

n = 10
C = 950
goods = []
max_weight = 200
max_value = 100


def good_gen():
    for i in range(n):
        goods.append((random.randint(1, max_weight), random.randint(1, max_value), i))


def timer(func):
    def wrapper(*args, **kwargs):
        start = time.time()
        optimal, lst = func(*args, **kwargs)
        end = time.time()
        print(f"{func.__name__} took {end - start} seconds")
        print(f"Optimal value: {optimal}")
        print(f"Optimal list: {lst}\n")

    return wrapper


def divide_conquer_recursion(i=0, remain=C, optimal_list=None):
    if optimal_list is None:
        optimal_list = []

    if i == n:
        return 0, optimal_list  # optimal_list is empty if no good is taken

    if goods[i][0] > remain:
        return divide_conquer_recursion(i + 1, remain, optimal_list)  # cannot putting it in, so we visit next

    else:  # can put it in
        not_put_it_in, not_in_list = divide_conquer_recursion(i + 1, remain,
                                                              optimal_list)  # not putting it in, so we visit next
        optimal_list_copy = optimal_list.copy()  # copy optimal_list
        optimal_list_copy.append(i)  # put it in
        later_values, in_list = divide_conquer_recursion(i + 1,
                                                         remain - goods[i][0], optimal_list_copy)  # putting it in
        put_it_in = goods[i][1] + later_values

        if put_it_in > not_put_it_in:
            return put_it_in, in_list  # return the better value and the list
        else:
            return not_put_it_in, not_in_list


@timer
def divide_conquer():
    return divide_conquer_recursion(0, C)


@timer
def dynamic_method():
    package_list = [[[] for j in range(C + 1)] for i in range(n + 1)]
    # dp[i][j] means the max value we can get if we put the first i goods in the bag with capacity j
    dp = [[0 for j in range(C + 1)] for i in range(n + 1)]

    for i in range(1, n + 1):  # i is the number of goods
        for j in range(1, C + 1):  # j is the capacity
            if goods[i - 1][0] > j:  # if the weight of the current good is larger than the capacity
                package_list[i][j] = package_list[i - 1][j].copy()  # we can not put the good in
                dp[i][j] = dp[i - 1][j]  # we can only take the previous one

            else:  # if the weight of the current good is smaller than the capacity
                put_it_in = goods[i - 1][1] + dp[i - 1][j - goods[i - 1][0]]  # we put the good in
                not_put_it_in = dp[i - 1][j]  # we don't put the good in

                if put_it_in > not_put_it_in:  # if we can put the good in
                    package_list[i][j] = package_list[i - 1][j - goods[i - 1][0]].copy()  # we put the good in
                    package_list[i][j].append(i - 1)  # we put the good in
                    dp[i][j] = put_it_in  # we put the good in
                else:  # if we can't put the good in
                    package_list[i][j] = package_list[i - 1][j]  # we don't put the good in
                    dp[i][j] = not_put_it_in  # we don't put the good in

    return dp[n][C], package_list[n][C]  # return the optimal value and the optimal list


def take_unit_value(elem):
    return elem[1] / elem[0]


@timer
def greedy_method():
    optimal_list = []
    goods_copy = goods.copy()  # copy the goods list
    goods_copy.sort(key=take_unit_value, reverse=True)  # sort the goods by the unit value
    capacity = C  # the capacity is the same as the bag
    value_all = 0  # the value of all the goods taken into the bag

    for good in goods_copy:  # for each good
        if capacity >= good[0]:  # if the capacity is larger than the weight of the good
            capacity -= good[0]  # reduce the capacity
            value_all += good[1]  # add the value of the good
            optimal_list.append(good[2])  # add the index of the good to the optimal list

    return value_all, optimal_list  # return the optimal value and the optimal list


@timer
def back_tracking():
    goods_copy = goods.copy()  # copy the goods list
    goods_copy.sort(key=take_unit_value, reverse=True)  # sort the goods by the unit value
    rem_sum = [0]  # the sum of the remaining value of the goods

    for i in range(n-1, -1, -1):  # for each good
        rem_sum.insert(0, rem_sum[0] + goods_copy[i][1])  # add the weight of the good to the sum

    # level is the level of the recursion,
    # cur_weight is the weight of the bag,
    # cur_val is the value of the bag,
    # optimal is the optimal value
    def dfs(level, cur_weight, cur_val, optimal, package_list):
        if level == n:
            return max(optimal, cur_val), package_list

        good = goods_copy[level]
        flag = 0
        package_list_copy = package_list.copy()

        if cur_weight + good[0] <= C:  # if the weight of the bag is larger than the weight of the good
            package_list_copy.append(good[2])  # add the index of the good to the optimal list
            optimal, left_list = dfs(level + 1, cur_weight + good[0],
                                     cur_val + good[1], optimal, package_list_copy)  # we can put the good in
            left_optimal = optimal
            optimal_list = left_list
            flag = 1

        # if the value of the bag is larger than the sum of the remaining value of the goods
        if cur_val + rem_sum[level + 1] > optimal:
            # we can not put the good in
            optimal, right_list = dfs(level + 1, cur_weight, cur_val, optimal, package_list)
            if flag == 0:
                return optimal, right_list
            flag = 2

        if flag == 0:
            return optimal, package_list
        elif flag == 1:
            return optimal, left_list
        else:
            if left_optimal >= optimal:
                return left_optimal, left_list
            else:
                return optimal, right_list

    return dfs(0, 0, 0, 0, [])


@timer
def branch_and_bound_method():
    goods_copy = goods.copy()  # copy the goods list
    goods_copy.sort(key=take_unit_value, reverse=True)  # sort the goods by the unit value

    optimal = 0
    queue = [(0.0, 0, 0, 0, [])]
    heapq.heapify(queue)  # min-root heap
    return_list = []

    while queue:  # while the queue is not empty
        item = heapq.heappop(queue)  # get the item with the minimum value
        upper_bound, index, cur_weight, cur_val, cur_list = item[:]  # get the upper bound, the index of the good, the weight of the bag, and the value of the bag
        upper_bound = - upper_bound  # get the upper bound

        if optimal < cur_val:  # if the upper bound is larger than the optimal value
            optimal = cur_val
            return_list = cur_list

        if index == n or upper_bound < optimal:  # if the index is the last good or the upper bound is smaller than the optimal value
            continue  # we can not put the good in

        weight, value, identity = goods_copy[index]  # get the weight and value of the good
        upper_bound = cur_val + (C - cur_weight) * value / weight  # get the upper bound

        if cur_weight + weight <= C:  # if the weight of the bag is larger than the weight of the good
            cur_list_copy = cur_list.copy()  # copy the optimal list
            cur_list_copy.append(identity)  # add the index of the good to the optimal list
            heapq.heappush(queue, (-upper_bound, index + 1, cur_weight + weight,
                                   cur_val + value, cur_list_copy))  # put the good in

        heapq.heappush(queue, (-upper_bound, index + 1, cur_weight, cur_val, cur_list))  # not put the good in

    return optimal, return_list


@timer
def monte_carlo_method():
    optimal = 0
    best_list = []

    for i in range(100000):  # for each iteration
        good_list = []  # the list of the goods taken into the bag
        random_list = random.sample(range(n), n)  # get a random list of the goods
        value = 0
        weight = 0
        for j in range(n):  # for each good
            if weight + goods[random_list[j]][0] > C:
                break
            value += goods[random_list[j]][1]
            weight += goods[random_list[j]][0]
            good_list.append(random_list[j])

            if value > optimal:  # if the value of the bag is larger than the optimal value
                optimal = value
                best_list = good_list

    return optimal, best_list


def main():
    print('number of goods: ' + str(n))
    print('Content of package: ' + str(C))
    print('Max weight: ' + str(max_weight))
    print('max_value: ' + str(max_value))
    good_gen()
    divide_conquer()
    dynamic_method()
    greedy_method()
    back_tracking()
    branch_and_bound_method()
    monte_carlo_method()


if __name__ == '__main__':
    main()
