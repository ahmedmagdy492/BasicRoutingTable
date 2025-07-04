from mtrie import Mtrie, Node

def main():
    routing_table = Mtrie()
    routing_table.insert("1.2.3.4/32")
    routing_table.insert("128.2.3.4/32")
    routing_table.insert("0.0.0.0/0")
    routing_table.insert("128.2.0.0/16")
    routing_table.insert("128.128.0.0/16")
    routing_table.insert("1.0.0.0/8")
    # routing_table.insert("64.128.128.0/24")

    routing_table.delete_route("1.2.3.4/32")

    routing_table.traverse(routing_table.root())
    print("\n\n")

    route: Node | None = routing_table.route_lookup("128.2.3.5")
    print(route)

    route: Node | None = routing_table.route_lookup("1.2.3.5")
    print(route)

    route: Node | None = routing_table.route_lookup("10.1.1.1")
    print(route)

    # Delete Test Cases
    # routing_table.delete_route("128.2.3.4/32")
    # routing_table.delete_route("0.0.0.0/0")
    # routing_table.delete_route("128.2.0.0/16")
    # routing_table.delete_route("128.128.0.0/16")
    # routing_table.delete_route("1.0.0.0/8")

    # routing_table.traverse(routing_table.root())
    # print("\n")

    # answer = routing_table.is_route_exist("128.128.0.2/32")
    # print("128.128.0.2/32", "->", answer)


main()