
class NetUtils:
    @staticmethod
    def ipstr_decimal_to_binstr(ipstr):
        parts = ipstr.split('.')
        bin_str = ""

        for part in parts:
            value = int(part)
            temp = str(bin(value)[2:])
            new_len = 8 - len(temp)
            padding = '0' * new_len
            bin_str += padding + temp

        return bin_str

    @staticmethod
    def calc_wildcard(mask_value):
        no_of_zeros = 32 - mask_value
        bin_str = mask_value * '0'
        bin_str += no_of_zeros * '1'
        return bin_str