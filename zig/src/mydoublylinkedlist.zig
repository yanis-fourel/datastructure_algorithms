const std = @import("std");
const Allocator = std.mem.Allocator;
const assert = std.debug.assert;

pub fn MyDoublyLinkedList(comptime T: type) type {
    return struct {
        const Self = @This();
        pub const Node = struct {
            val: T,
            prev: ?*Node = null,
            next: ?*Node = null,
        };

        first: ?*Node = null,
        last: ?*Node = null,

        pub fn append(self: *Self, node: *Node) void {
            assert(node.next == null);
            assert(node.prev == null);

            if (self.empty()) {
                self.first = node;
                self.last = node;
                return;
            } else {
                self.insertAfter(self.last.?, node);
            }
        }

        pub fn prepend(self: *Self, node: *Node) void {
            assert(node.next == null);
            assert(node.prev == null);

            if (self.empty()) {
                self.first = node;
                self.last = node;
                return;
            }
            self.first.?.prev = node;
            node.next = self.first;
            self.first = node;
        }

        pub fn insertAfter(self: *Self, node: *Node, new_node: *Node) void {
            assert(new_node.next == null);
            assert(new_node.prev == null);

            if (self.last == node) {
                self.last = new_node;
            }
            new_node.next = node.next;
            new_node.prev = node;
            node.next = new_node;
        }

        pub fn popFirst(self: *Self) ?*Node {
            if (self.first) |first| {
                self.remove(first);
                return first;
            }
            return null;
        }

        pub fn popLast(self: *Self) ?*Node {
            if (self.last) |last| {
                self.remove(last);
                return last;
            }
            return null;
        }

        pub fn empty(self: Self) bool {
            return self.first == null;
        }

        pub fn remove(self: *Self, node: *Node) void {
            assert(!self.empty());
            if (node.prev) |prev| {
                prev.next = node.next;
            } else {
                self.first = self.first.?.next;
            }
            if (node.next) |next| {
                next.prev = node.prev;
            } else {
                self.last = self.last.?.prev;
            }
            node.prev = null;
            node.next = null;
        }
    };
}

test "some tests" {
    const L = MyDoublyLinkedList(u8);
    var ll = L{};

    var one = L.Node{ .val = 1 };
    var two = L.Node{ .val = 2 };
    var three = L.Node{ .val = 3 };
    var four = L.Node{ .val = 4 };
    var five = L.Node{ .val = 5 };

    try std.testing.expect(ll.empty());
    try std.testing.expectEqual(null, ll.popFirst());
    try std.testing.expectEqual(null, ll.popLast());
    ll.append(&one);
    try std.testing.expect(!ll.empty());
    try std.testing.expectEqual(one.val, ll.popFirst().?.val);
    try std.testing.expect(ll.empty());
    ll.append(&two);
    try std.testing.expect(!ll.empty());
    try std.testing.expectEqual(two.val, ll.popLast().?.val);
    try std.testing.expect(ll.empty());

    ll.append(&one);
    ll.append(&two);
    ll.append(&three);
    ll.append(&four);
    ll.append(&five);
    try std.testing.expectEqual(five.val, ll.popLast().?.val);
    try std.testing.expectEqual(one.val, ll.popFirst().?.val);
    try std.testing.expectEqual(four.val, ll.popLast().?.val);
    try std.testing.expectEqual(two.val, ll.popFirst().?.val);
    try std.testing.expectEqual(three.val, ll.popLast().?.val);
    try std.testing.expect(ll.empty());

    ll.prepend(&five);
    ll.prepend(&four);
    ll.prepend(&three);
    ll.prepend(&two);
    ll.prepend(&one);
    try std.testing.expectEqual(one.val, ll.popFirst().?.val);
    try std.testing.expectEqual(five.val, ll.popLast().?.val);
    try std.testing.expectEqual(two.val, ll.popFirst().?.val);
    try std.testing.expectEqual(four.val, ll.popLast().?.val);
    try std.testing.expectEqual(three.val, ll.popFirst().?.val);
    try std.testing.expect(ll.empty());

    ll.append(&three);
    ll.append(&four);
    ll.prepend(&two);
    ll.prepend(&one);
    ll.append(&five);

    ll.remove(&four);
    ll.remove(&two);
    try std.testing.expectEqual(one.val, ll.popFirst().?.val);
    try std.testing.expectEqual(five.val, ll.popLast().?.val);
    try std.testing.expectEqual(three.val, ll.popFirst().?.val);
    try std.testing.expect(ll.empty());

    ll.append(&one);
    ll.append(&three);
    ll.insertAfter(&three, &five);
    ll.insertAfter(&three, &four);
    ll.insertAfter(&one, &two);
    try std.testing.expectEqual(one.val, ll.popFirst().?.val);
    try std.testing.expectEqual(two.val, ll.popFirst().?.val);
    try std.testing.expectEqual(three.val, ll.popFirst().?.val);
    try std.testing.expectEqual(four.val, ll.popFirst().?.val);
    try std.testing.expectEqual(five.val, ll.popFirst().?.val);
    try std.testing.expect(ll.empty());
}
