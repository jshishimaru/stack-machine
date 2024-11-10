document.addEventListener('DOMContentLoaded', () => {
    let currentIndex = 0;
    let instructions = [];
    let memoryString = '';

    fetch('output.txt')
        .then(response => response.text())
        .then(data => {
            const lines = data.split('\n');
            let currentInstruction = {};

            lines.forEach(line => {
                if (line.startsWith('Memory:')) {
                    memoryString = line.replace('Memory:', '').trim();
                } else if (line.startsWith('Current Instruction:')) {
                    currentInstruction.currentInstruction = line.replace('Current Instruction:', '').trim();
                } else if (line.startsWith('Data Stack:')) {
                    currentInstruction.dataStack = line.replace('Data Stack:', '').trim();
                } else if (line.startsWith('Return Stack:')) {
                    currentInstruction.returnStack = line.replace('Return Stack:', '').trim();
                } else if (line.startsWith('PC:')) {
                    currentInstruction.pc = parseInt(line.replace('PC:', '').trim(), 10);
                    instructions.push(currentInstruction);
                    currentInstruction = {};
                }
            });

            updateMemoryDisplay(memoryString, instructions[0].pc);
            updateDisplay();
        })
        .catch(error => console.error('Error fetching output.txt:', error));

    document.getElementById('next-button').addEventListener('click', () => {
        if (currentIndex < instructions.length - 1) {
            currentIndex++;
            updateDisplay();
        }
    });

    document.getElementById('previous-button').addEventListener('click', () => {
        if (currentIndex > 0) {
            currentIndex--;
            updateDisplay();
        }
    });

    function updateDisplay() {
        const instruction = instructions[currentIndex];
        document.getElementById('current-instruction').textContent = instruction.currentInstruction || '';

        updateMemoryDisplay(memoryString, instruction.pc);
        updateStackDisplay('data-stack', instruction.dataStack);
        updateStackDisplay('return-stack', instruction.returnStack);
    }

    function updateMemoryDisplay(memoryString, pc) {
		pc--;
        const memoryElement = document.getElementById('memory');
        memoryElement.innerHTML = ''; // Clear previous memory items

        const memoryItems = memoryString.split(',').reverse();
        memoryItems.forEach((item, index) => {
            const memoryItemElement = document.createElement('div');
            memoryItemElement.className = 'stack-item';
            memoryItemElement.textContent = item.trim();
            if (index === memoryItems.length - 1 - pc) {
                memoryItemElement.innerHTML += ' <span class="pc-arrow">← PC</span>'; // Add arrow to indicate PC
            }
            memoryElement.appendChild(memoryItemElement);
        });
    }

    function updateStackDisplay(elementId, stackData) {
        const stackElement = document.getElementById(elementId);
        stackElement.innerHTML = ''; // Clear previous stack items

        if (stackData) {
            const stackItems = stackData.split(' ');
            stackItems.forEach(item => {
                const stackItemElement = document.createElement('div');
                stackItemElement.className = 'stack-item';
                stackItemElement.textContent = item;
                stackElement.appendChild(stackItemElement);
            });
        }
    }
});