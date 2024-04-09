#demo of how the tree works
    heap->insert("a", 10, &overflow);
    heap->insert("b", 9, &overflow);
    heap->insert("b", 9, &overflow);

    if (overflow)
        std::cout << "Some values are missimng due to the limited size" << std::endl;

    Node min = heap->extractMin();
    std::cout << heap->traverse(0) << std::endl;