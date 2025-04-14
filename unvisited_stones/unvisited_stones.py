def unvisited_stones(N: int, stones: int, birds: list[int]) -> int:
    """
    Находит количество валунов, которые не посетит ни одна птица.

    :param N: Количество птиц.
    :param stones: Общее количество валунов.
    :param birds: Список с дистанциями перелёта каждой птицы.
    :return: Количество непосещённых валунов.
    """
    
    visited = [False] * (stones + 1)
    for bird in birds:
        for pos in range(bird, stones + 1, bird):
            visited[pos] = True
    return sum(1 for v in visited[1:] if not v)