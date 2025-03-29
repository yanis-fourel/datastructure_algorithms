const std = @import("std");
const MyDoublyLinkedList = @import("./mydoublylinkedlist.zig").MyDoublyLinkedList;
const Allocator = std.mem.Allocator;

/// TODO: Do we ever want to use a SinglyLinkedList over a RingBuffer?
pub fn MyQueue(comptime T: type) type {
    return struct {
        const Self = @This();
        // Could be a Singly linked list
        const L = MyDoublyLinkedList(T);

        ll: L,
        allocator: Allocator,

        pub fn init(allocator: Allocator) Self {
            return Self{ .ll = L{}, .allocator = allocator };
        }

        pub fn deinit(self: *Self) void {
            _ = self;
        }

        pub fn push(self: *Self, item: T) !void {
            // TODO: is there a way to not allocate each node individually?
            // Would using an ArrayList instead be faster? Until how many items?
            // Or maybe that depends on the allocator?
            const n = try self.allocator.create(L.Node);
            n.* = L.Node{ .val = item };
            self.ll.append(n);
        }

        pub fn pop(self: *Self) ?T {
            if (self.ll.first) |first| {
                const res = first.*.val;
                self.ll.remove(first);
                self.allocator.destroy(first);
                return res;
            }
            return null;
        }

        pub fn empty(self: Self) bool {
            return self.ll.first == null;
        }
    };
}

test "MyQueue" {
    const allocator = std.testing.allocator;
    var q = MyQueue(u8).init(allocator);
    defer q.deinit();

    try std.testing.expect(q.empty());
    try q.push(0);
    try std.testing.expect(!q.empty());
    try std.testing.expectEqual(0, q.pop());
    try std.testing.expect(q.empty());
    try q.push(1);
    try q.push(2);
    try q.push(3);
    try std.testing.expectEqual(1, q.pop());
    try std.testing.expectEqual(2, q.pop());
    try std.testing.expectEqual(3, q.pop());
    try std.testing.expect(q.empty());
}
