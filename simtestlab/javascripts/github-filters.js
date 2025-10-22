/**
 * GitHub-style table filtering for issue tables
 */

document.addEventListener('DOMContentLoaded', function() {
    // Initialize filters for all issue tables
    initializeIssueTableFilters();
});

function initializeIssueTableFilters() {
    const tables = document.querySelectorAll('.github-issue-table');
    tables.forEach(table => {
        addFiltersToTable(table);
    });
}

function addFiltersToTable(table) {
    const container = table.parentElement;
    
    // Create filter controls container
    const filterContainer = document.createElement('div');
    filterContainer.className = 'issue-filters';
    filterContainer.innerHTML = createFilterHTML(table);
    
    // Insert filters before the table container (outside the table UI)
    container.parentElement.insertBefore(filterContainer, container);
    
    // Add event listeners
    setupFilterEventListeners(filterContainer, table);
    
    // Initialize filter state
    updateFilterCounts(table);
}

function createFilterHTML(table) {
    // Check if this is an overview table (has Repository column) or repository table
    const isOverviewTable = table && table.rows.length > 0 && table.rows[0].cells.length > 6;
    return `
        <div class="filter-controls">
            <div class="filter-group">
                <button class="filter-btn active" data-filter="all">
                    <span class="filter-icon">📄</span>
                    <span class="filter-count" data-count="all">0</span> All
                </button>
                <button class="filter-btn" data-filter="open">
                    <span class="filter-icon">🟢</span>
                    <span class="filter-count" data-count="open">0</span> Open
                </button>
                <button class="filter-btn" data-filter="closed">
                    <span class="filter-icon">🔴</span>
                    <span class="filter-count" data-count="closed">0</span> Closed
                </button>
            </div>
            
            <div class="filter-group">
                <div class="search-container">
                    <input type="text" class="search-input" placeholder="Search issues..." />
                    <span class="search-icon">🔍</span>
                </div>
            </div>
            
            <div class="filter-group">
                <select class="assignee-filter" data-filter="assignee">
                    <option value="">All assignees</option>
                </select>
            </div>
            
            <div class="filter-group">
                <select class="sort-select">
                    <option value="created-desc">Newest</option>
                    <option value="created-asc">Oldest</option>
                    <option value="title-asc">Title A-Z</option>
                    <option value="title-desc">Title Z-A</option>
                </select>
            </div>
        </div>
    `;
}

function setupFilterEventListeners(filterContainer, table) {
    // Status filter buttons
    const statusButtons = filterContainer.querySelectorAll('.filter-btn[data-filter]');
    statusButtons.forEach(btn => {
        btn.addEventListener('click', () => {
            // Remove active class from all buttons
            statusButtons.forEach(b => b.classList.remove('active'));
            // Add active class to clicked button
            btn.classList.add('active');
            
            applyFilters(filterContainer, table);
        });
    });
    
    // Search input
    const searchInput = filterContainer.querySelector('.search-input');
    searchInput.addEventListener('input', debounce(() => {
        applyFilters(filterContainer, table);
    }, 300));
    
    // Dropdown filters
    const dropdownFilters = filterContainer.querySelectorAll('select[data-filter]');
    dropdownFilters.forEach(select => {
        select.addEventListener('change', () => {
            applyFilters(filterContainer, table);
        });
    });
    
    // Sort select
    const sortSelect = filterContainer.querySelector('.sort-select');
    sortSelect.addEventListener('change', () => {
        applyFilters(filterContainer, table);
    });
    
    // Populate dropdown options
    populateDropdownOptions(filterContainer, table);
}

function populateDropdownOptions(filterContainer, table) {
    const rows = Array.from(table.querySelectorAll('tbody tr'));
    
    // Check if this is an overview table (has Repository column) or repository table
    const isOverviewTable = rows.length > 0 && rows[0].cells.length > 6;
    const assigneeColumnIndex = isOverviewTable ? 4 : 3;
    
    // Collect unique assignees
    const assignees = new Set();
    
    rows.forEach(row => {
        const assigneeCell = row.cells[assigneeColumnIndex]?.textContent.trim();
        
        if (assigneeCell && assigneeCell !== '-') {
            assigneeCell.split(',').forEach(assignee => {
                assignees.add(assignee.trim().replace(/\+\d+$/, ''));
            });
        }
    });
    
    // Populate assignee dropdown
    const assigneeSelect = filterContainer.querySelector('.assignee-filter');
    assignees.forEach(assignee => {
        const option = document.createElement('option');
        option.value = assignee;
        option.textContent = assignee;
        assigneeSelect.appendChild(option);
    });
}

function applyFilters(filterContainer, table) {
    const rows = Array.from(table.querySelectorAll('tbody tr'));
    
    // Check if this is an overview table (has Repository column) or repository table
    const isOverviewTable = rows.length > 0 && rows[0].cells.length > 6;
    const assigneeColumnIndex = isOverviewTable ? 4 : 3;
    
    // Get filter values
    const activeStatusFilter = filterContainer.querySelector('.filter-btn.active')?.dataset.filter;
    const searchTerm = filterContainer.querySelector('.search-input').value.toLowerCase();
    const assigneeFilter = filterContainer.querySelector('.assignee-filter').value;
    const sortBy = filterContainer.querySelector('.sort-select').value;
    
    // Filter rows
    let filteredRows = rows.filter(row => {
        // Status filter
        if (activeStatusFilter !== 'all') {
            const statusCell = row.cells[0]?.textContent.toLowerCase();
            const isOpen = statusCell.includes('open');
            if (activeStatusFilter === 'open' && !isOpen) return false;
            if (activeStatusFilter === 'closed' && isOpen) return false;
        }
        
        // Search filter
        if (searchTerm) {
            const rowText = Array.from(row.cells).map(cell => cell.textContent.toLowerCase()).join(' ');
            if (!rowText.includes(searchTerm)) return false;
        }
        
        // Assignee filter
        if (assigneeFilter) {
            const assigneeText = row.cells[assigneeColumnIndex]?.textContent || '';
            if (!assigneeText.includes(assigneeFilter)) return false;
        }
        
        return true;
    });
    
    // Sort rows
    filteredRows = sortRows(filteredRows, sortBy);
    
    // Hide all rows first
    rows.forEach(row => {
        row.style.display = 'none';
    });
    
    // Show filtered rows
    filteredRows.forEach((row, index) => {
        row.style.display = '';
        // Move to correct position
        table.querySelector('tbody').appendChild(row);
    });
    
    // Update filter counts
    updateFilterCounts(table, filteredRows);
}

function sortRows(rows, sortBy) {
    // Check if this is an overview table (has Repository column) or repository table
    const isOverviewTable = rows.length > 0 && rows[0].cells.length > 6;
    const titleColumnIndex = isOverviewTable ? 3 : 2;
    const dateColumnIndex = isOverviewTable ? 6 : 5;
    
    return rows.sort((a, b) => {
        switch (sortBy) {
            case 'created-desc':
                return new Date(b.cells[dateColumnIndex]?.textContent) - new Date(a.cells[dateColumnIndex]?.textContent);
            case 'created-asc':
                return new Date(a.cells[dateColumnIndex]?.textContent) - new Date(b.cells[dateColumnIndex]?.textContent);
            case 'title-asc':
                return (a.cells[titleColumnIndex]?.textContent || '').localeCompare(b.cells[titleColumnIndex]?.textContent || '');
            case 'title-desc':
                return (b.cells[titleColumnIndex]?.textContent || '').localeCompare(a.cells[titleColumnIndex]?.textContent || '');
            default:
                return 0;
        }
    });
}

function updateFilterCounts(table, filteredRows = null) {
    const allRows = Array.from(table.querySelectorAll('tbody tr'));
    const visibleRows = filteredRows || allRows.filter(row => row.style.display !== 'none');
    
    const openCount = visibleRows.filter(row => 
        row.cells[0]?.textContent.toLowerCase().includes('open')
    ).length;
    
    const closedCount = visibleRows.filter(row => 
        row.cells[0]?.textContent.toLowerCase().includes('closed')
    ).length;
    
    // Update counts in filter buttons (filters are now outside the container)
    const filterContainer = table.parentElement.previousElementSibling;
    if (filterContainer && filterContainer.classList.contains('issue-filters')) {
        const allCountSpan = filterContainer.querySelector('[data-count="all"]');
        const openCountSpan = filterContainer.querySelector('[data-count="open"]');
        const closedCountSpan = filterContainer.querySelector('[data-count="closed"]');
        
        if (allCountSpan) allCountSpan.textContent = visibleRows.length;
        if (openCountSpan) openCountSpan.textContent = openCount;
        if (closedCountSpan) closedCountSpan.textContent = closedCount;
    }
}

// Utility function for debouncing
function debounce(func, wait) {
    let timeout;
    return function executedFunction(...args) {
        const later = () => {
            clearTimeout(timeout);
            func(...args);
        };
        clearTimeout(timeout);
        timeout = setTimeout(later, wait);
    };
}