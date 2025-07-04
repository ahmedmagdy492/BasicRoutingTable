from net_utils import NetUtils


class Node:
    def __parse_cidr(self, cidr):
        return cidr.split("/")

    def __init__(self, cidr):
        parsed_cidr = self.__parse_cidr(cidr)
        self.prefix_len = int(parsed_cidr[1])
        self.prefix = NetUtils.ipstr_decimal_to_binstr(parsed_cidr[0])
        self.wildcard = NetUtils.calc_wildcard(self.prefix_len)
        self.data = cidr
        self.child: dict[str, Node | None] = { '0': None, '1': None, 'x': None }

    def __str__(self):
        return self.data

    def calc_effprefix(self):
        effprefix = ""

        for i in range(len(self.wildcard)):
            if self.wildcard[i] == '0':
                effprefix += self.prefix[i]
            else:
                effprefix += 'x'

        return effprefix


class Mtrie:
    def __diff_strings(self, str1: str, str2: str) -> int:

        for i in range(len(str1)):
            if str1[i] != str2[i]:
                return i

        return -1

    def __init__(self):
        self.__root: Node = Node("0/0")

    def insert(self, cidr):
        new_node: Node = Node(cidr)
        eff_prefix: str = new_node.calc_effprefix()

        if self.__root.child[eff_prefix[0]] is None:
            self.__root.child[eff_prefix[0]] = new_node
        else:
            cur: Node | None = self.__root.child[eff_prefix[0]]

            new_node_effprefix: str = new_node.calc_effprefix()

            while cur is not None:
                cur_effprefix = cur.calc_effprefix()
                if cur_effprefix != new_node_effprefix:
                    split_offset = self.__diff_strings(cur_effprefix, new_node_effprefix)
                    daughter = Node("0/0")

                    daughter.prefix_len = cur.prefix_len - split_offset
                    daughter.prefix = cur.prefix[split_offset:]
                    daughter.wildcard = cur.wildcard[split_offset:]
                    daughter.data = cur.data
                    daughter.child = cur.child
                    cur.child = {daughter.calc_effprefix()[0]: daughter}

                    cur.prefix_len = split_offset
                    cur.data = None
                    cur.prefix = cur.prefix[:split_offset]
                    cur.wildcard = cur.wildcard[:split_offset]

                    niece = Node("0/0")
                    niece.prefix_len = 32 - split_offset
                    niece.prefix = new_node.prefix[split_offset:]
                    niece.wildcard = new_node.wildcard[split_offset:]
                    niece.data = cidr

                    cur.child[niece.calc_effprefix()[0]] = niece

                    break
                cur = cur.child[eff_prefix[0]]

    def root(self):
        return self.__root

    def is_route_exist(self, cidr: str):
        if self.__root is None:
            return False

        search_node: Node = Node(cidr)
        search_node_eff_prefix: str = search_node.calc_effprefix()
        cur_node: Node = self.__root.child[search_node_eff_prefix[0]]
        cur_node_eff_prefix: str = cur_node.calc_effprefix()
        cur_offset = 0

        while search_node_eff_prefix[cur_offset:cur_offset+cur_node.prefix_len] == cur_node_eff_prefix:
            cur_offset += cur_node.prefix_len
            if cur_offset == 32:
                return True
            cur_node = cur_node.child[search_node_eff_prefix[cur_offset]]
            if cur_node is None:
                break
            cur_node_eff_prefix = cur_node.calc_effprefix()

        return False

    def find_node_parent_by_cidr(self, cidr: str, root: Node | None) -> tuple[Node, Node] | None:
        if not root:
            return None

        for child in root.child.values():
            if not child:
                continue
            if child.data == cidr:
                return (root, child)
            ret_node = self.find_node_parent_by_cidr(cidr, child)
            if ret_node != None:
                return ret_node

        return None
        

    def delete_route(self, cidr: str) -> bool:
        search_res : tuple[Node, Node] | None = self.find_node_parent_by_cidr(cidr, self.__root)

        if search_res is None:
            return False

        node_to_delete_parent, child_to_delete = search_res
        search_effprefix: str = child_to_delete.calc_effprefix()

        if node_to_delete_parent.child[search_effprefix[0]] is None:
            return False

        other_child_node: Node | None = None
        node_to_delete_parent.child[search_effprefix[0]] = None

        if '0' in node_to_delete_parent.child.keys() and node_to_delete_parent.child['0'] is not None:
            other_child_node = node_to_delete_parent.child['0']
        elif '1' in node_to_delete_parent.child.keys() and node_to_delete_parent.child['1'] is not None:
            other_child_node = node_to_delete_parent.child['1']
        else:
            other_child_node = node_to_delete_parent.child['x']

        if node_to_delete_parent == self.__root:
            return True

        node_to_delete_parent.prefix_len += other_child_node.prefix_len
        node_to_delete_parent.prefix += other_child_node.prefix
        node_to_delete_parent.wildcard += other_child_node.wildcard
        node_to_delete_parent.data = other_child_node.data
        node_to_delete_parent.child = other_child_node.child

        return True


    @staticmethod
    def traverse(root: Node):
        if not root:
            return

        print(root.data, end=" ")
        print("->", root.calc_effprefix() + "/" + str(root.prefix_len))

        for child in root.child.values():
            Mtrie.traverse(child)

