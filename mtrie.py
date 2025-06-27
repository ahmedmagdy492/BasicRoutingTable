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

    @staticmethod
    def traverse(root: Node):
        if not root:
            return

        print(root.data, end=" ")
        print("->", root.calc_effprefix() + "/" + str(root.prefix_len))

        for child in root.child.values():
            Mtrie.traverse(child)

